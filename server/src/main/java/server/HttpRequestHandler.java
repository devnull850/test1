package server;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.OutputStream;

import static java.util.stream.Collectors.joining;

public class HttpRequestHandler implements HttpHandler {

    private static final int SIZE = 0x2f;
    private static final String FILENAME = "res/blob";

    private byte[] getBlobBytes() throws IOException {
        byte[] b = new byte[SIZE];

        try (FileInputStream in = new FileInputStream(FILENAME)) {
            if (in.read(b, 0, SIZE) == SIZE) {
                return b;
            }
        }

        return null;
    }

    @Override
    public void handle(HttpExchange httpExchange) throws IOException {
        if (httpExchange.getRequestMethod().equals("GET")) {
            OutputStream body = null;

            try {
                body = httpExchange.getResponseBody();
                byte[] blob = getBlobBytes();
                httpExchange.sendResponseHeaders(200, blob.length);
                body.write(blob);

                for (int i = 0; i < 2; ++i) {
                    body.write(0xd);
                    body.write(0xa);
                }
            }
            finally {
                if (body != null) {
                    body.flush();
                    body.close();
                }
            }
        }
    }

}

