package crow;
import crow.CrowPublisher;

public class CrowSubscriber extends CrowPublisher {
	public void on_message(byte[] arr) {
		System.out.println("on_message (need to reimplement)");
	}

	private long instance;
	public CrowSubscriber() {
		instance = nativeNew();
	}
	native private long nativeNew();

	native public void subscribe();
}
