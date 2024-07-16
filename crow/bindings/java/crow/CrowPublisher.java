package crow;
import crow.CrowNode;

public class CrowPublisher extends CrowNode {
	private long instance;
	public CrowPublisher() {
		instance = nativeNew();
	}
	native private long nativeNew();

	native public void set_crowker_address(String addr);
	native public void set_theme(String thm);
	native public void publish(byte[] data);
	native public void publish(HostAddr addr, int nodeno,
	                           String theme, byte[] data,
	                           int qos, int ackquant);

}
