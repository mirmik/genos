package crow;
import crow.Crow;

public class CrowFinalSubscriber extends CrowSubscriber
{
	@Override
	public void on_message(byte[] arr) {
		System.out.println("CrowFinalSubscriber.on_message");
		Crow.stop_spin(false);
	}
}