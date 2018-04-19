package application;

import java.util.HashMap;

public class OversiktPrinter {

	public static void main(String[] args) {
		HashMap<String, String> oversiktOverHvemSomHarGjortHva = new HashMap<>();

		oversiktOverHvemSomHarGjortHva.put("Ingebrigt Nyg�rd", "Skrevet og feils�kt kj�rekode og -algoritme.");
		oversiktOverHvemSomHarGjortHva.put("Asbj�rn Toft", "Skrevet kode for avstandssensorer og feils�kt kj�rekode. Har koblet opp p� poluluen. Personlig polulu-trener #2.");
		oversiktOverHvemSomHarGjortHva.put("Ask Berstad Kolltveit", "Satt opp git-hub for gruppa, satt p� ekstra vekt p� poluluen. Oppslagsverk for C++. Personlig polulu-trener #1.");
		oversiktOverHvemSomHarGjortHva.put("K�re Obrestad", "Generell feils�king, arbeidet med bluetooth-tilkobling. Strategiansvarlig.");
		oversiktOverHvemSomHarGjortHva.put("Sebastian Vitters�", "Laget FXML og javakode for bluetooth.");
		
		for (String key : oversiktOverHvemSomHarGjortHva.keySet()) {
		    System.out.println(key + ": " + oversiktOverHvemSomHarGjortHva.get(key));
		}
	}
}
