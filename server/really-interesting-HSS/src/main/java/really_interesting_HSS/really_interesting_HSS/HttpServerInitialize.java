package really_interesting_HSS.really_interesting_HSS;

import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.http.HttpServerCodec;
import io.netty.handler.ssl.SslContext;

public class HttpServerInitialize extends ChannelInitializer<SocketChannel> {
	private final SslContext sslCtx;
	
	public HttpServerInitialize(SslContext sslCtx) {
		this.sslCtx = sslCtx;
	}
	
	@Override
	public void initChannel(SocketChannel ch) {
		
		ChannelPipeline p = ch.pipeline();

		if (sslCtx != null) p.addLast(sslCtx.newHandler(ch.alloc()));

		p.addLast(new HttpServerCodec());
		p.addLast(new HttpServerHandler());
	}
}

