package crow;

public class HostAddr
{
	private long instance;
	native private long nativeNew(String str);
	public HostAddr(String str)
	{
		instance = nativeNew(str);
	}
}