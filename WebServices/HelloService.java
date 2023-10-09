package pcc;

/**
 * "Hello, World!" SOAP Service
 */
public class HelloService {

  /**
  *  Says Hello to Client
  */
  public String sayHello(String name) {
    return new String ("Hello, " + name + "!");
  }
}
