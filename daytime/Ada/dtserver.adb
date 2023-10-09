with Ada.Command_Line;
with Ada.Streams;
with Ada.Text_IO;
with GNAT.Sockets;

use  Ada.Command_Line;
use  Ada.Streams;
use  Ada.Text_IO;

use type GNAT.Sockets.Selector_Status;

procedure DTServer is
   Port            : GNAT.Sockets.Port_Type := 13;
   Server_Socket   : GNAT.Sockets.Socket_Type;
   Server_Endpoint : GNAT.Sockets.Sock_Addr_Type;
   Client_Socket   : GNAT.Sockets.Socket_Type;
   Client_Endpoint : GNAT.Sockets.Sock_Addr_Type;
   Accept_Status   : GNAT.Sockets.Selector_Status;
   
   Buffer_Size : constant := 1024;
   Buffer : Stream_Element_Array(1 .. Buffer_Size);
   Last   : Stream_ELement_Offset;
begin
   -- Do we have an explict port number? If so override the default.
   if Argument_Count = 1 then
      Port := GNAT.Sockets.Port_Type'Value(Argument(1));
   end if;
   
   -- Create a socket.
   GNAT.Sockets.Create_Socket(Server_Socket);
   
   -- Set up the server address.
   Server_Endpoint.Addr := GNAT.Sockets.Any_Inet_Addr;
   Server_Endpoint.Port := Port;
   
   -- Bind.
   GNAT.Sockets.Bind_Socket(Server_Socket, Server_Endpoint);
   
   -- Listen.
   GNAT.Sockets.Listen_Socket(Server_Socket);
   
   loop
      -- Accept a new connection.
      GNAT.Sockets.Accept_Socket
        (Server  => Server_Socket,
         Socket  => Client_Socket,
         Address => Client_Endpoint,
         Timeout => GNAT.Sockets.Forever,
         Status  => Accept_Status);
      
      -- Ignore error conditions (for now).
      if Accept_Status = GNAT.Sockets.Completed then
         
         -- Compute the client address in string form and the text of the data to send.
         declare
            Printable_Address : String := GNAT.Sockets.Image(Client_Endpoint.Addr);
            Message : String := "Hello, World!";  -- TODO: Look up date and time.
         begin
            Put_Line("Accepted a connection from " & Printable_Address);
            
            -- Prepare the message as an array of stream elements.
            for I in Message'Range loop
               Buffer(Stream_Element_Offset(I)) := Character'Pos(Message(I));
            end loop;
            
            -- Send the message.
            GNAT.Sockets.Send_Socket
              (Socket => Client_Socket,
               Item   => Buffer(1 .. Message'Length),
               Last   => Last);
            
            if Last /= Message'Length then
               Put_Line("ERROR: Message send incomplete!");
            end if;
         end;
         GNAT.Sockets.Close_Socket(Client_Socket);
      end if;
      
   end loop;

end DTServer;
