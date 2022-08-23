
void(* resetFunc) (void) = 0;//объявляем функцию reset с адресом 0

void ASSERT()
 {
   resetFunc();
 }