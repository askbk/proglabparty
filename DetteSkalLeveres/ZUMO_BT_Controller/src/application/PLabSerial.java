package application;

import java.io.IOException;
import java.io.OutputStream;
import java.lang.reflect.Method;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;

import javafx.application.Platform;

/** 
 * @author Dag Svanaes, IDI, NTNU.
 * PLabSerial is a class for simplifying communication with serial ports.
 * It was made for the PLab course at IDI-NTNU to ease communication 
 * with serial Bluetooth devices, such as HC-06.
 * It build on a library from com.fazecast.jSerialComm.
 *
 * Methods:
 *   public String[] getPortNames()
 *   public boolean openPort(String portName)
 *   public boolean openPLabPort()
 *   public String getOpenPortName() 
 *   public void closePort()
 *   public void sendMessage(String s)
 *   public void sendMessage(String s, int i)
 *   public void addListener(Object receiverObject, String methodName)
 *     Requires that the receiverObject class implements a callback method:
 *         public void methodName(String message, int value) 
 *
 */

public class PLabSerial {
	
	private SerialPort activePort = null;
	private static Class<?>[] parameterType = {String.class, Integer.TYPE};

	private Object callbackReceiverObject = null;
	private String callbackMethodName = null;
	
	private String inputBuffer = "";
	
	//PLab_serial() {
	//}

	/**
	 * Gets the port names
	 * @return the port names
	 */
	public String[] getPortNames() {
		SerialPort[] serialports = SerialPort.getCommPorts();
    	String[] portNames = new String[serialports.length];
		int length = serialports.length;
		for(int i=0;i<length;i++) {
			portNames[i] = serialports[i].getDescriptivePortName();
		};
		return portNames;
	}
	
	/**
	 * Opens a serial port
	 * @param portName the port name
	 * @return success (if the port existed and was opened)
	 */
	public boolean openPort(String portName) {
		String[] portNames = getPortNames();
		int i = java.util.Arrays.asList(portNames).indexOf(portName);
		if (i > -1) {
			closePort();
			activePort = SerialPort.getCommPorts()[i];
			activePort.openPort();
		    activePort.addDataListener(new SerialPortDataListener() {  // anonymous inner class
				   @Override
				   public int getListeningEvents() { return SerialPort.LISTENING_EVENT_DATA_AVAILABLE; }
				   @Override
				   public void serialEvent(SerialPortEvent event)
				   {
				      if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE)
				         return;
				      byte[] newData = new byte[activePort.bytesAvailable()];
				      int numRead = activePort.readBytes(newData, newData.length);
				 //     System.out.println("Read " + numRead + " bytes.");
//				      String s = ""; ;
//				      for(int i=0;i<numRead;i++) {
//							s += (char) newData[i];
//				      };
				      inputBuffer += new String(newData, 0, numRead);
				      emptyInputBuffer();
				   }
			  });		
			return true;
		} else {
			return false;
		}
	}
	
	public boolean openPLabPort() {
		String portName;
		String[] serialportNames = getPortNames();
		for(int i=0;i<serialportNames.length;i++) {
			portName = serialportNames[i];
			int plab_found = portName.indexOf("PLab");
			int dialin_found = portName.indexOf("Dial-In");
			if ((plab_found > -1) && (dialin_found == -1)) {
				openPort(portName);
				return true;
			}
		};
		return false;
	}
	
	public String getOpenPortName() {
		if (activePort == null) {
			return null;
		} else {
			return activePort.getDescriptivePortName();
		}
	}
	
	public void closePort() {
		if (activePort != null) {
			activePort.closePort();
			activePort = null;
		}
	}
	
	private void sendStringMessage(String s) {
		s = s + "\r\n";
		if (activePort != null) {
	   	  byte[] buffer = new byte[s.length()];
		  for(int i=0;i<s.length();i++) {
			  buffer[i] = (byte) s.charAt(i);
		  };
		  OutputStream out =  activePort.getOutputStream();
		  try {
			  out.write(buffer,0,s.length());
		  } catch (IOException e) {
			  // TODO Auto-generated catch block
			// e.printStackTrace();
		  } 
	    }
	}
	
	public void sendMessage(String s, int i) {
		sendStringMessage(s + "," + i);
	}
	
	public void sendMessage(String s) {
		sendStringMessage(s);
	}
	public void addListener(Object receiverObject, String methodName) {
		callbackReceiverObject = receiverObject;
		callbackMethodName = methodName;
	}
	
	private void callback(String message, int value) {
		if (callbackReceiverObject != null && callbackMethodName != null) {
			Method m = null;
			try {
				m = callbackReceiverObject.getClass().getMethod(callbackMethodName, parameterType);
			} catch (Exception e) {
			}
			if (m != null) {
				try {
					Object[] args = {message, value};
					m.invoke(callbackReceiverObject, args);
				} catch (Exception e) {
				}
				return;
			}
	     }
    }

	private void emptyInputBuffer() {
		String message = "";
		int lineEndPosition = inputBuffer.indexOf("\r\n");
		if (lineEndPosition > -1) {
			if (lineEndPosition == 0) {
				message = "";
			} else {
				message = inputBuffer.substring(0,lineEndPosition);
			};
			inputBuffer = inputBuffer.substring(lineEndPosition+2);
			int commaposition = message.indexOf(",");
			int messageParameter = 0;
			if (commaposition > -1) {
				String number = message.substring(commaposition+1);
				message = message.substring(0,commaposition);
			    try {
			    	 messageParameter = Integer.parseInt(number);
			        } catch (NumberFormatException n) {
			         messageParameter = 0;
			        }
			};
			final String finalMessage = message;
			final int finalmessageParameter = messageParameter;
			boolean runLater = Platform.isFxApplicationThread();
			if (runLater) {
				Platform.runLater(new Runnable() {
					@Override
					public void run() {
						try {
							callback(finalMessage,finalmessageParameter);
						} catch (Exception e) {
						}
					}
				});
			} else {
				callback(message,messageParameter);
			};
			emptyInputBuffer();
		}
	}
}