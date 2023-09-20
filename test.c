#include <stdio.h>
#include "calculadora.h" // Colocar Arquivos Header referênciando a aplicação principal
#include "unity.h" // Esse é o header que pegamos ao clonar o repositório do Unity

void setUp(){
    //Passos anteriores à execução do evento de testes
}

void tearDown(){
    //Passos depois da execução do evento de testes
}

//Teste da função Soma.
void test_FuncaoSoma(){
    int a = 10;
    int b = 20;
    int resultado = soma(a,b);
    int esperado = 30;
    TEST_ASSERT_EQUAL_INT(esperado,resultado);
}
//Teste da função subtração
void test_FuncaoSubtracao(){
    int a = 11;
    int b = 1;
    int resultado = subtracao(a,b);
    int esperado = 2;
    TEST_ASSERT_EQUAL_INT(esperado,resultado);
}

int main(){
    UNITY_BEGIN();
    RUN_TEST(test_FuncaoSoma);
    RUN_TEST(test_FuncaoSubtracao);
    return UNITY_END();
}