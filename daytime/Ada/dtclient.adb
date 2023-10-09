with Ada.Command_Line;
with Ada.Streams;
with Ada.Text_IO;
with GNAT.Sockets;

use  Ada.Command_Line;
use  Ada.Streams;
use  Ada.Text_IO;

procedure DTClient is
   Port   : GNAT.Sockets.Port_Type := 13;
   Socket : GNAT.Sockets.Socket_Type;
   Server_Endpoint : GNAT.Sockets.Sock_Addr_Type;
   
   Buffer_Size : constant := 1024;
   Buffer : Stream_Element_Array(1 .. Buffer_Size);
   Last   : Stream_ELement_Offset;
begin
   -- Do we have an appropriate number of command line arguments?
   if Argument_Count < 1  or Argument_Count > 2 then
      Put("Usage: "); Put(Command_Name); Put_Line(" IPAddress [port]");
      Set_Exit_Status(Failure);
      return;
   end if;
   
   -- Do we have an explict port number? If so override the default.
   if Argument_Count = 2 then
      Port := GNAT.Sockets.Port_Type'Value(Argument(2));
   end if;
   
   -- Create a socket.
   GNAT.Sockets.Create_Socket(Socket);
   
   -- Set up the server address.
   Server_Endpoint.Addr := GNAT.Sockets.Inet_Addr(Argument(1));
   Server_Endpoint.Port := Port;
   
   -- Connect to the server.
   GNAT.Sockets.Connect_Socket(Socket, Server_Endpoint);
   
   -- Read the data.
   loop
      GNAT.Sockets.Receive_Socket(Socket, Buffer, Last);
      exit when Last = 0;
      for I in 1 .. Last loop
         Put(Character'Val(Buffer(I)));
      end loop;
   end loop;
   GNAT.Sockets.Close_Socket(Socket);
end DTClient;
