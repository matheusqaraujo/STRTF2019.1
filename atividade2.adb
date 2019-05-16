-- Atividade 2
-- Equipe: Caio Cavalcante, Marcos Levi, Matheus Queiroz

With Ada.Integer_Text_IO, Ada.Text_IO, Ada.Calendar;
Use Ada.Integer_Text_IO, Ada.Text_IO, Ada.Calendar;

procedure atividade2 is
   FlagA : Integer:= 1;
   FlagB : Integer:= 0;
   FlagC : Integer:= 0;
   indice: Integer:= 1;
   vetor: array(1..30) of Character;

   task type Thread_A is
   task body Thread_A is
   begin
      while indice < 29 loop
         while FlagA = 0 loop
            NULL;
         end loop;
         vetor(indice):='a';
         indice:=indice+1;
         FlagA:=0;
         FlagB:=1;
      end loop;     
   end Thread_A;
   
   task type Thread_B is
   task body Thread_B is
   begin
      while indice < 30 loop
         while FlagB = 0 loop
            NULL;
         end loop;
         vetor(indice):='b';
         indice:=indice+1;
         FlagB:=0;
         FlagC:=1;
      end loop;     
   end Thread_B;
   
   task type Thread_C is
   task body Thread_C is
   begin
      while indice < 31 loop
         while FlagC = 0 loop
            NULL;
         end loop;
         vetor(indice):='c';
         indice:=indice+1;
         FlagC:=0;
         FlagA:=1;
      end loop;
      for i in 1..30 loop
         Put(vetor(i));
      end loop;
   end Thread_C;
   
   A : Thread_A;
   B : Thread_B;
   C : Thread_C;
  
   begin
   null;
end atividade2;   
