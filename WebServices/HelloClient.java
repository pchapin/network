
/**
 * "Hello, World!" SOAP Client
 * usage:  java HelloClient first_name
*/

import java.net.*;
import java.util.Vector;
import org.apache.soap.SOAPException;
import org.apache.soap.Fault;
import org.apache.soap.Constants;
import org.apache.soap.rpc.Call;
import org.apache.soap.rpc.Parameter;
import org.apache.soap.rpc.Response;

public class HelloClient {

  /**
   * Static Main method
   */
  public static void main(String[] args) {
    String firstName = args[0];
    System.out.println("Hello SOAP Client");
    HelloClient helloClient = new HelloClient(  );
    try {
      String greeting = helloClient.getGreeting(firstName);
      System.out.print(greeting);
    }
    catch (SOAPException e) {
      String faultCode = e.getFaultCode(  );
      String faultMsg = e.getMessage(  );
      System.err.println("SOAPException Thrown (details below):");
      System.err.println("FaultCode: " + faultCode);
      System.err.println("FaultMessage: " + faultMsg);
    }
    catch (MalformedURLException e) {
      System.err.println(e);
    }
  }

  /**
   * getGreeting Method
  */
  public String getGreeting(String firstName)
    throws SOAPException, MalformedURLException {

    //  Create SOAP RPC Call Object
    Call call = new Call(  );

    // Set Encoding Style to standard SOAP encoding
    call.setEncodingStyleURI(Constants.NS_URI_SOAP_ENC);

    // Set Object URI and Method Name
    call.setTargetObjectURI("urn:pccHello");
    call.setMethodName("sayHello");

    //  Set Method Parameters
    Parameter param = new Parameter(
      "name",
      String.class,
      firstName,
      Constants.NS_URI_SOAP_ENC);

    Vector paramList = new Vector(  );
    paramList.addElement(param);
    call.setParams(paramList);

    //  Set the URL for the Web Service
    URL url = new URL("http://localhost:8081/soap/servlet/rpcrouter");

    // Invoke the Service
    Response resp = call.invoke(url, "");

    // Check for Faults
    if (!resp.generatedFault(  )) {
      // Extract Return value
      Parameter result = resp.getReturnValue(  );
      String greeting = (String) result.getValue(  );
      return greeting;
    }
    else {
      //  Extract Fault Code and String
      Fault f = resp.getFault(  );
      String faultCode = f.getFaultCode(  );
      String faultString = f.getFaultString(  );
      System.err.println("Fault Occurred (details follow):");
      System.err.println("Fault Code: " + faultCode);
      System.err.println("Fault String: " + faultString);
      return new String ("Fault Occurred. No greeting for you!");
    }
  }
}
