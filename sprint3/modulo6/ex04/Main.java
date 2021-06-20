package ex04;

class Passenger {
	public final String address;
	private int number_passangers;
	public boolean transported;

	public Passenger(String address, int number_passangers) {
		this.address = address;
		this.number_passangers = number_passangers;
	}

	public String getAddress() {
		return address;
	}

	public int getNumber_passangers() {
		return number_passangers;
	}

	public void setNumber_passangers(int number_passangers) {
		this.number_passangers = number_passangers;
	}

	public synchronized int pickPassangers(int number_seats) {
		while (number_passangers > 0) {
			int aux = number_passangers;
			if (number_seats >= number_passangers) {
				number_passangers = 0;
				transported = true;
			} else {
				number_passangers -= number_seats;
			}
			System.out.println((aux - number_passangers) + " passengers are being transported.");
		}
		return number_seats;
	}
}

public class Main {
	public static void main(String[] args) throws InterruptedException {
		TaxiCentral taxi_central = new TaxiCentral();

		int number_taxis = 2;
		Thread[] nt = new Thread[number_taxis];
		for (int i = 0; i < number_taxis; i++) {
			int number = (int) (Math.random() * 4 + 1); //Number of seats
			nt[i] = new Thread(taxi_central.new Taxi(number)); //Create taxis with the seat's number above
			System.out.println("Number of seats: " + number);
			nt[i].start();
		}

		taxi_central.newPassenger(new Passenger("ISEP", 6));

		for (int i = 0; i < number_taxis; i++) {
			Thread.sleep(1000);
			System.out.println("Interruption.");
			nt[i].interrupt();
		}

		System.out.println("Main thread finished.");
	}
}
