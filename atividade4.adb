-- Atividade 4 - N versões (Voto Majoritário em ADA)
-- Equipe: Caio Cavalcante, Marcos Levi, Matheus Queiroz

with Ada.Integer_Text_IO, Ada.Text_IO, Ada.Calendar;
use Ada.Integer_Text_IO, Ada.Text_IO, Ada.Calendar;

with Ada.Task_Identification; use Ada.Task_Identification;

procedure n_versoes is
   canal : array (1 .. 6) of Integer := (-1, -1, -1, -1, -1, -1);
   type vetor_int is array (1 .. 3) of Integer;

   function compara (versaoErrada :    out Integer;
      vetorvoto                   : in     vetor_int) return Integer
   is
   begin
      if vetorvoto (1) = vetorvoto (2) and vetorvoto (2) = vetorvoto (3) then
         versaoErrada := -1;
         return vetorvoto (1);
      end if;
      
      if ((vetorvoto (1) /= vetorvoto (2)) and (vetorvoto (2) = vetorvoto (3)))
      then
         versaoErrada := 0;
         return vetorvoto (2);
      end if;

      if ((vetorvoto (1) = vetorvoto (3)) and (vetorvoto (2) /= vetorvoto (3)))
      then
         versaoErrada := 1;
         return vetorvoto (1);
      end if;
      
      if ((vetorvoto (1) = vetorvoto (2)) and (vetorvoto (2) /= vetorvoto (3)))
      then
         versaoErrada := 2;
         return vetorvoto (1);
      end if;

      return 0;
   end compara;

   function send_assync (buf : in Integer; can : in Integer) return Integer is
   begin
      canal (can) := buf;
      return 0;
   end send_assync;

   function receive (buf : out Integer; can : in Integer) return Integer is
   begin
      while (canal (can) = -1) loop
         null;
      end loop;
      buf         := canal (can);
      canal (can) := -1;
      return 0;
   end receive;

   task type thread_code_a is

   end thread_code_a;
   task body thread_code_a is
      voto : Integer := 5+5;
      x    : Integer;
      status    : Integer;
   begin
      x := send_assync (voto, 1);
      x := receive (status, 4);
      if (status = 1) then
         Put_Line ("Versao TA falhou!");
         
      else
         Put_Line ("Versao TA correta!");
         
      end if;
   end thread_code_a;

   task type thread_code_b is

   end thread_code_b;
   task body thread_code_b is
      voto : Integer := 2*5;
      x    : Integer;
      status    : Integer;
   begin
      x := send_assync (voto, 2);
      x := receive (status, 5);
      if (status = 1) then
         Put_Line ("Versao TB falhou!");
         
      else
         Put_Line ("Versao TB correta!");
         
      end if;
   end thread_code_b;

   task type thread_code_c is
   end thread_code_c;
   task body thread_code_c is
      voto : Integer := 3+7;
      x    : Integer;
      status    : Integer;
   begin
      x := send_assync (voto, 3);
      x := receive (status, 6);
      if (status = 1) then
         Put_Line ("Versao TC falhou!");
        
      else
         Put_Line ("Versao TC correta!");
        
      end if;
   end thread_code_c;

   task type thread_driver is
   end thread_driver;
   task body thread_driver is
      statusok    : Integer := 0;
      statuserror : Integer := 1;
      vetorvoto   : vetor_int;
      versaoErro  : Integer;
      votoMaj     : Integer;
      x           : Integer;
   begin
      for i in Integer range 1 .. 3 loop
         x := receive (vetorvoto (i), i);
      end loop;

      votoMaj := compara (versaoErro, vetorvoto);
      if (versaoErro = -1) then
         x := send_assync (statusok, 4);
         x := send_assync (statusok, 5);
         x := send_assync (statusok, 6);
      end if;
      if (versaoErro = 0) then
         x := send_assync (statuserror, 4);
         x := send_assync (statusok, 5);
         x := send_assync (statusok, 6);
      end if;
      if (versaoErro = 1) then
         x := send_assync (statusok, 4);
         x := send_assync (statuserror, 5);
         x := send_assync (statusok, 6);
      end if;
      if (versaoErro = 2) then
         x := send_assync (statusok, 4);
         x := send_assync (statusok, 5);
         x := send_assync (statuserror, 6);
      end if;

      New_Line;
      Put_Line("Voto marjoritario eh: " & Integer'Image (votoMaj));
      New_Line;
   end thread_driver;

   a      : thread_code_a;
   b      : thread_code_b;
   c      : thread_code_c;
   driver : thread_driver;

begin
   null;

end n_versoes;
