package ex04;

import java.util.*;

public class TaxiCentral {
	private final ArrayList<Passenger> waiting_list = new ArrayList<>();

	public ArrayList<Passenger> getWaiting_list() {
		return waiting_list;
	}

	//Requests for taxi
	public synchronized void newPassenger(Passenger passenger) {
		this.waiting_list.add(passenger);
	}

	public synchronized Passenger getNextTaxi() {
		if (!waiting_list.isEmpty()) {
			return waiting_list.get(0);
		} else {
			return null;
		}
	}

	public synchronized void removeTaxi(Passenger passenger) {
		waiting_list.remove(passenger);
	}

	public class Taxi implements Runnable {
		public int number_seats;

		public Taxi(int number_seats) {
			this.number_seats = number_seats;
		}

		public int getNumber_seats() {
			return number_seats;
		}

		public void setNumber_seats(int number_seats) {
			this.number_seats = number_seats;
		}

		@Override
		public void run() {
			while (!Thread.interrupted()) {
				Passenger passenger = getNextTaxi();
				if (passenger != null) {
					int aux = passenger.pickPassangers(number_seats);
					if (passenger.transported == true) {
						removeTaxi(passenger);
					}
					try {
						Thread.sleep(1000);
						synchronized (System.out) {
							System.out.println("Passenger transported to " + passenger.address);
						}
					} catch (InterruptedException e) {
						break;
					}
				}
			}
		}
	}
}