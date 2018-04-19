package application;

import java.util.HashMap;

public class OversiktPrinter {

	public static void main(String[] args) {
		HashMap<String, String> oversiktOverHvemSomHarGjortHva = new HashMap<>();

		oversiktOverHvemSomHarGjortHva.put("Ingebrigt Nygård", "Skrevet og feilsøkt kjørekode og -algoritme.");
		oversiktOverHvemSomHarGjortHva.put("Asbjørn Toft", "Skrevet kode for avstandssensorer og feilsøkt kjørekode. Har koblet opp på poluluen. Personlig polulu-trener #2.");
		oversiktOverHvemSomHarGjortHva.put("Ask Berstad Kolltveit", "Satt opp git-hub for gruppa, satt på ekstra vekt på poluluen. Oppslagsverk for C++. Personlig polulu-trener #1.");
		oversiktOverHvemSomHarGjortHva.put("Kåre Obrestad", "Generell feilsøking, arbeidet med bluetooth-tilkobling. Strategiansvarlig.");
		oversiktOverHvemSomHarGjortHva.put("Sebastian Vittersø", "Laget FXML og javakode for bluetooth.");
		
		for (String key : oversiktOverHvemSomHarGjortHva.keySet()) {
		    System.out.println(key + ": " + oversiktOverHvemSomHarGjortHva.get(key));
		}
	}
}
