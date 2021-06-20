package ex02;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.Semaphore;

import static ex02.ex02.*;

class thread_test extends Thread {
    @Override
    public void run() {

        System.out.println(thread_test.currentThread().getName() + " is running.");
        while (number_tickets > 0 || !Thread.interrupted()) {
            try {
                sem_control.acquire();
            } catch (InterruptedException e) {
                break;
            }
            if (number_tickets <= 0) {
                break;
            }
            number_tickets--;
            System.out.println("Remaining tickets: " + number_tickets + " by " + thread_test.currentThread().getName());
            sem_control.release();
        }
    }
}

public class ex02 extends Thread {
    public static int number_tickets = 100; //shared variable
    static final Semaphore sem_control = new Semaphore(1);

    public static void main(String[] args) throws InterruptedException {
        int sellers = 5; //number threads
        Thread[] ts = new Thread[sellers];
        for (int i = 0; i < sellers; i++) {
            ts[i] = new Thread(new thread_test());
            ts[i].start();
        }

        for (int i = 0; i < sellers; i++) {
            Thread.sleep(1000);
            System.out.println("Interruption!");
            ts[i].interrupt();
        }

        System.out.println("Main thread finished.");
    }
}