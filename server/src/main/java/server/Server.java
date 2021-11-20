package server;

import com.sun.net.httpserver.HttpServer;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Server {

    public static void main(String[] args) throws IOException {
        HttpServer server =
                HttpServer.create(new InetSocketAddress("localhost",8080), 0);
        server.createContext("/", new HttpRequestHandler());
        ExecutorService pool = Executors.newSingleThreadExecutor();
        server.setExecutor(pool);
        server.start();
    }

}

