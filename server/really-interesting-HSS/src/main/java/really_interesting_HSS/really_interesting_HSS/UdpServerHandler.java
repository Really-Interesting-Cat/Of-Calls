package really_interesting_HSS.really_interesting_HSS;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.DataLine;
import javax.sound.sampled.SourceDataLine;
import javax.sound.sampled.TargetDataLine;

import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.channel.socket.DatagramPacket;

public class UdpServerHandler extends SimpleChannelInboundHandler<DatagramPacket> {

	//AudioFormat format = new AudioFormat(AudioFormat.Encoding.PCM_SIGNED, 44100.0F, 16, 2, 4, 44100.0F, false);
	AudioFormat format = new AudioFormat(16000.0F, 16, 1, true, false);
	//AudioFormat format = new AudioFormat(AudioFormat.Encoding.PCM_SIGNED, 44100.0F, 16, 1, 2, 44100.0F, false);
	DataLine.Info sourceInfo = new DataLine.Info(SourceDataLine.class, format);
	DataLine.Info targetInfo = new DataLine.Info(TargetDataLine.class, format);

	//byte[] buffer;
	SourceDataLine source;
	TargetDataLine target;

	public UdpServerHandler() throws Exception {
		source = (SourceDataLine) AudioSystem.getLine(sourceInfo);
		source.open(format);
		source.start();

		target = (TargetDataLine) AudioSystem.getLine(targetInfo);
		target.open(format);
		target.start();
	}

	@Override
	public void channelRead0(ChannelHandlerContext ctx, DatagramPacket msg) throws Exception {

		ByteBuf in = Unpooled.buffer();
		ByteBuf out = Unpooled.buffer();

		in = msg.content();
		//byte[] buffer = new byte[in.readableBytes()];
		byte[] readBuffer = new byte[in.readableBytes()];
		byte[] writeBuffer = new byte[in.readableBytes()];
		System.out.println(in.readableBytes());
		in.readBytes(readBuffer);
		//source.write(buffer, 0, buffer.length);
		source.write(readBuffer, 0, readBuffer.length);

		//target.read(buffer, 0, buffer.length);
		target.read(writeBuffer, 0, writeBuffer.length);
		out.writeBytes(writeBuffer, 0, writeBuffer.length);

		//ctx.writeAndFlush(new DatagramPacket(out, msg.sender()));
		ctx.writeAndFlush(new DatagramPacket(out, msg.sender()));
		//ctx.writeAndFlush(new DatagramPacket(Unpooled.copiedBuffer(buffer), msg.sender()));
	}

}
