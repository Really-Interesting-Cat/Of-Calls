package really_interesting_HSS.really_interesting_HSS;

import io.netty.bootstrap.Bootstrap;
import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.Channel;
import io.netty.channel.ChannelOption;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioDatagramChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.logging.LogLevel;
import io.netty.handler.logging.LoggingHandler;
import io.netty.handler.ssl.SslContext;
import io.netty.handler.ssl.SslContextBuilder;
import io.netty.handler.ssl.util.SelfSignedCertificate;

public class App {

	private int udpPort, httpPort;
	EventLoopGroup udpGroup;
	EventLoopGroup httpBossGroup;
	EventLoopGroup httpWorkerGroup;

	public App(int udpPort, int httpPort) {
		this.udpPort = udpPort;
		this.httpPort = httpPort;
	}

	public Bootstrap getUdpBootstrap() throws Exception {
		udpGroup = new NioEventLoopGroup(15);

		Bootstrap b = new Bootstrap();

		b.group(udpGroup)
		 .channel(NioDatagramChannel.class)
		 .handler(new UdpServerInitialize());

		return b;
	}

	public ServerBootstrap getHttpBootstrap() throws Exception {
		httpBossGroup = new NioEventLoopGroup(1);
		httpWorkerGroup = new NioEventLoopGroup(2);

		final boolean SSL = System.getProperty("ssl") != null;
		

		final SslContext sslCtx;

		if (SSL) {
			SelfSignedCertificate ssc = new SelfSignedCertificate();
			sslCtx = SslContextBuilder.forServer(ssc.certificate(), ssc.privateKey()).build();
		} else {
			sslCtx = null;
		}

		ServerBootstrap b = new ServerBootstrap();

		b.option(ChannelOption.SO_BACKLOG, 1024)
		 .group(httpBossGroup, httpWorkerGroup)
		 .channel(NioServerSocketChannel.class)
		 .handler(new LoggingHandler(LogLevel.INFO))
		 .childHandler(new HttpServerInitialize(sslCtx));

		return b;
	}

	public void run(Bootstrap udpBootstrap, ServerBootstrap httpBootstrap) throws Exception {
		Channel udpChannel = udpBootstrap.bind(udpPort).sync().channel();
		Channel httpChannel = httpBootstrap.bind(httpPort).sync().channel();

		udpChannel.closeFuture().sync();
		httpChannel.closeFuture().sync();
		
		udpGroup.shutdownGracefully();
		httpBossGroup.shutdownGracefully();
		httpWorkerGroup.shutdownGracefully();
	}

	public static void main(String[] args) throws Exception {
		App app = new App(3000, 8080);
		app.run(app.getUdpBootstrap(), app.getHttpBootstrap());
	}
}
