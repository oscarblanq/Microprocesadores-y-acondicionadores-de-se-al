const int T1 = 20, T2 = 22, T3 = 27, T4 = 25;
const int Rele = 3, Cal = 2;
int AN4 = A4, TempAct = 0;

void setup(){
    pinMode(Ref, OUTPUT);
    pinMode(Cal, OUTPUT);
    pinMode(AN4, INPUT);
}

int leeTemperatura(void){
    AN4 = analogRead(A4);   //Conexión analógica del LM35
    AN4 = (AN4*500)/1024;   //Calcula la temperatura en ºC
    return (AN4);
}

void loop(){
    tempAct = leeTemperatura();

    if(TempAct < T1){   //Si la temperatura es mejor a T1 = 20ºC

    digitalWrite(Cal, HIGH);    //Activa el rele del Calefactor
    digitalWrite(Ref, LOW);     //Desactiva el Refrigerador
    while(leeTemperatura < T2) delay(180000);
    digitalWrite(Cal, LOW);     //Desactiva el rele del Calefactor
    }

    else if(TempAct > T3){      //Si la temperatura es mayor a T3 = 27ºC
    digitalWrite(Ref, HIGH);    //Activa el rele del Refrigerador
    digitalWrite(Cal, LOW);
    while(leeTemperatura() > T4) delay(180000);
    digitalWrite(Ref, LOW);     //Desactiva el rele del Refrigerador
    }

    else{       //En cualquier otro caso, ambos reles están desactivados
    digitalWrite(Ref, LOW);
    digitalWrite(Cal, LOW);
    }

    delay(180000);      //Retardo entre medición y medición de 3min

}
