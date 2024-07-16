package crow;

import crow.Crow;
import crow.CrowSpamSubscriber;

public class CrowTest {
	public static void main(String[] args) {
		Crow.diagnostic(true);

		Crow.create_udpgate(12, 10010);

		CrowSpamSubscriber subscriber = new CrowSpamSubscriber();
		subscriber.bind(12);

		//Crow.start_spin();
		//Crow.join_spin();
	}
}