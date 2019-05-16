-- Atividade 7 - Redundância de software com semáforo em ADA
-- Equipe: Caio Cavalcante, Marcos Levi, Matheus Queiroz

With Ada.Text_IO; Use Ada.Text_IO;
With Ada.Integer_Text_IO, Ada.Text_IO, Ada.Calendar;
Use Ada.Integer_Text_IO, Ada.Text_IO, Ada.Calendar;

Procedure atividade7 is
   canal : array(1..6) of integer := (-1, -1, -1, -1, -1, -1);
   chanAB: Integer := 1;
   chanBA: Integer := 2;
   chanAC: Integer := 3;
   chanCA: Integer := 4;
   chanBC: Integer := 5;
   chanCB: Integer := 6;
   t: Integer;

   protected type Resource is
      entry Seize;
      procedure Release;
   private Busy : Boolean := False;
   end Resource;

   protected body Resource is
      entry Seize when not Busy is
      begin
         Busy := True;
      end Seize;
      procedure Release is
      begin
         Busy := False;
      end Release;
   end Resource;

   sem1 : array(1..6) of Resource;

   function send_assync(mensagem: in integer; indice: in integer) return integer is
   begin
      canal(indice) := mensagem;
      sem1(indice).Release;
      return 0;
   end send_assync;

   function receive(mensagem : out integer; indice : in integer) return Integer is
   begin
      sem1(indice).Seize;
      mensagem := canal(indice);
      return 0;
   end receive;
   
   function fa(a : in Integer) return Integer is
   begin
      return 10;
   end fa;
   
   function fb(a : in Integer) return Integer is
   begin
      return 500;
   end fb;
   
   function fc(a : Integer) return Integer is
   begin
      return 100;
   end fc;

task type TA;
   task body TA is
      x : Integer;
      y : Integer;
      status : Integer;
   begin
      delay 1.0;
      y := 5;
      while ((y>=1) and (y<=10)) loop
         delay 1.0;
         x := fa(1);
         Put_Line("TA executando Funcao A\n");
         t := send_assync(x,chanAB);
         t := receive(y,chanCA);
      end loop;
      delay 2.0;
      if y = -5 then
         y := 5;
         while ((y>=1) and (y<=10)) loop
            delay 1.0;
            x := fa(1);
            Put_Line("TA executando Funcao A\n");
            t := send_assync(x,chanAC);
            t := receive(y,chanCA);
         end loop;
         
         while 1=1 loop
            delay 1.0;
            t := fa(1);
            t := fb(1);
            t := fc(1);            
            Put_Line("TA executando Funcao A,B,C \n");
         end loop;
      else
         status := -5;
         t := send_assync(status,chanAB);
         t := receive(y,chanBA);
         while ((y>=50) and (y<=100)) loop
            delay 1.0;
            x := fa(1);
            t := fc(1);
            Put_Line("TA executando Funcao A e funcao C tambem!\n");
            t := send_assync(x,chanAB);
            t := receive(y,chanBA);
         end loop;
         while 1=1 loop
            delay 1.0;
            t := fa(1);
            t := fb(1);
            t := fc(1);
            Put_Line("TA executando Funcao A,B,C \n");
         end loop;
      end if;     
   end TA;  
   
task type TB;
   task body TB is
      x : Integer;
      y : Integer;
      status : Integer;

   begin
      delay 1.0;
      t := receive(y,chanAB);
      while ((y>=5) and (y<=10)) loop
         delay 1.0;
         x := fb(1);
         Put_Line("TB executando Funcao B\n");
         t := send_assync(x,chanBC);
         t := receive(y,chanAB);
      end loop;
      if y = -5 then
         y:=5;
         while ((y>=5) and (y<=10)) loop
            delay 1.0;
            x := fb(1);
            Put_Line("TB executando Funcao B\n");
            t := send_assync(x,chanBA);
            t := receive(y,chanAB);
         end loop;
         while 1=1 loop
            delay 1.0;
            t := fa(1);
            t := fb(1);
            t := fc(1);            
            Put_Line("TB executando Funcao A,B,C \n");
         end loop;
      else
         status := -5;
         t := send_assync(status,chanBC);
         t := receive(y,chanCB);
         while (y>=1) and (y<=10) loop
            delay 1.0;
            x := fb(1);
            t := fa(1);
            Put_Line("TB executando Funcao B e funcao A tambem!\n");
            t := send_assync(x,chanBC);
            t := receive(y,chanCB);
         end loop;
         while 1=1 loop
            delay 1.0;
            t := fa(1);
            t := fb(1);
            t := fc(1);
            Put_Line("TB executando Funcao A,B,C \n");
         end loop;
      end if;     
   end TB;  
   
task type TC;
   task body TC is
      x : Integer;
      y : Integer;
      status : Integer;

   begin
      delay 1.0;
      t := receive(y,chanBC);
      while (y>=50) and (y<=100) loop
         delay 1.0;
         x := fc(1);
         Put_Line("TC executando Funcao C\n");
         t := send_assync(x,chanCA);
         t := receive(y,chanBC);
      end loop;
      if y = -5 then
         y:=50;
         while (y>=50) and (y<=100) loop
            delay 1.0;
            x := fc(1);
            Put_Line("TC executando Funcao C\n");
            t := send_assync(x,chanCB);
            t := receive(y,chanBC);
         end loop;
         while 1=1 loop
            delay 1.0;
            t := fa(1);
            t := fb(1);
            t := fc(1);            
            Put_Line("TC executando Funcao A,B,C \n");
         end loop;
      else
         status := -5;
         t := send_assync(status,chanCA);
         t := receive(y,chanAC);
         while (y>=5) and (y<=10) loop
            delay 1.0;
            x := fc(1);
            t := fb(1);
            Put_Line("TC executando Funcao C e funcao B tambem!\n");
            t := send_assync(x,chanCA);
            t := receive(y,chanAC);
         end loop;
         while 1=1 loop
            delay 1.0;
            t := fa(1);
            t := fb(1);
            t := fc(1);
            Put_Line("TC executando Funcao A,B,C \n");
         end loop;
      end if;     
   end TC; 
   
   aa : TA;
   bb : TB;
   cc : TC;

begin
   t := 1;
   for k in 1..6 loop
      sem1(k).Seize;
   end loop;
end atividade7;
