package io.lefty.pbpipes;

import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.MessageOrBuilder;

/**
 * Prints a message.
 */
public final class BigQueryJsonFormat {

  public static JsonObject print(MessageOrBuilder message) throws InvalidProtocolBufferException {
    EnhancedJsonFormat.Printer printer = EnhancedJsonFormat.printer();
    printer.printInt64Numbers = true;
    String p = printer.print(message);
    return new JsonParser().parse(p).getAsJsonObject();
  }

  // do not instantiate.
  private BigQueryJsonFormat() {
  }
}
