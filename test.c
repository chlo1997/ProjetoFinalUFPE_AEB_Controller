#include <stdio.h>
#include "calculadora.h" // Colocar Arquivos Header referênciando a aplicação principal
#include "unity.h" // Esse é o header que pegamos ao clonar o repositório do Unity

void setUp(){
    //Steps before the tests executions
}

void tearDown(){
    //Steps after the tests executions
}

//Test Case 1: 
void test_Funcao1(){
    int a = 10;
    int b = 20;
    int resultado = soma(a,b);
    int esperado = 30;
    TEST_ASSERT_EQUAL_INT(esperado,resultado);
}
//Test Case 2: 
void test_Funcao2(){
    int a = 11;
    int b = 1;
    int resultado = subtracao(a,b); // atualizar as funções
    int esperado = 2;
    TEST_ASSERT_EQUAL_INT(esperado,resultado);
}
//Test Case 3: 
void test_Funcao3(){
    int a = 11;
    int b = 1;
    int resultado = subtracao(a,b); // Atualizar as funções
    int esperado = 2;
    TEST_ASSERT_EQUAL_INT(esperado,resultado);
}

int main(){
    UNITY_BEGIN();
    RUN_TEST(test_Funcao1);
    RUN_TEST(test_Funcao2);
    RUN_TEST(test_Funcao3);
    return UNITY_END();
}