package crow;

import java.nio.charset.StandardCharsets;

public class CrowSpamSubscriber //extends CrowNode
{
	private long instance;

	public void on_message(byte[] arr)
	{ 
		System.out.println("on_message (need to reimplement)");
	}

	native private long nativeNew();

	public CrowSpamSubscriber()
	{
		instance = nativeNew();
	}

	native public void bind(int no);

	native public void subscribe(int nid, String addr, int qos, int ackquant);
	native public void resubscribe(int qos, int ackquant);
}