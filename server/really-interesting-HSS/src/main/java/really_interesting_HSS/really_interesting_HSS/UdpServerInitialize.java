package really_interesting_HSS.really_interesting_HSS;

import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.nio.NioDatagramChannel;

public class UdpServerInitialize extends ChannelInitializer<NioDatagramChannel> {

	@Override
	protected void initChannel(NioDatagramChannel ch) throws Exception {
		ChannelPipeline p = ch.pipeline();
		p.addLast(new UdpServerHandler());
		//p.addLast(new LoggingHandler(LogLevel.INFO));
	}
	
}
