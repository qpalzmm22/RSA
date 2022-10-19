#include <iostream>
#include <vector>
#include <math.h>
#include <cstdlib>
#define SEED 123

using namespace std;

long long N;
long long private_key;
long long public_key;

long long ct[32]; 
char plain_text[32];
char deciphered_text[32];
int len = 0;

// finds gcd between two nums using euclidean algorithm
int gcd(long long n1, long long n2){
  if(n2 == 0)
    return n1;
  else
    return gcd(n2, n1 % n2);
}

// use eratos
bool is_prime(long long p){
  if(p <= 1)
    return false;
  if(p == 2 || p == 3)
    return true;
  
  for(int i = 2; i * i <= p; i++)
    if(p % i == 0)
      return false;

  return true;
}

bool are_primes(long long p, long long q){
  if(is_prime(p) && is_prime(q))
    return true;
  else
    return false;
}

void key_generation(long long p, long long q){
  // create phi(n)
  N = p * q;
  long long phi = (p - 1) * (q - 1);

  // generate random number with seed 123 until the number is coprime to phi(n)
  srand(123);
  while(1){
    long long rand_num = (rand()) ;
    //cout << rand_num << "\n";
    
    // they are coprime
    if( rand_num > 1 && rand_num < phi && gcd(rand_num, phi) == 1){
      public_key = rand_num;
      break;
    } 
      //cout << "pub-key) new number generating\n";
  }
  // find private key 
  while(1){
    long long rand_num = rand() % (phi-1) +2;
    
    if( (rand_num * public_key ) % phi == 1){
      private_key = rand_num;
      break;
    }
  }
  printf("pulbic key is %lld, private key is %lld\n\n", public_key, private_key);
}

long long my_pow(long long num, long long exp){
  long long result = 1;
  while(exp--){
    result *= num ;
  }
  return result;
}

long long my_modulus(long long num, long long exp, long long m){
  if(exp == 1){
    return num % m;
  }
  if(exp == 0){
    return 1;
  }
  long long tmp = exp;
  long long msb = 0;
  while(tmp >>= 1){
    msb++;
  }
  long long div = 1 << msb;
  long long rem = exp - div;
  long long ans;
  if(rem == 0){
    long long half = my_modulus(num, div/2, m);
    ans = (half * half) % m; 
  } else{
    ans = (my_modulus(num, div, m) * my_modulus(num, rem, m)) % m; 
  }
  //cout << "my_m : " << num  << " "<< exp << " " << ans << "\n" ;
  return ans;
}

void encrypt(){
  printf("\nASCII value of ciphertext is ");
  for(int i = 0 ; i < 32; i++){
    if(plain_text[i] == 0x0)
      break;
    len++;
    ct[i] = my_modulus(plain_text[i], public_key , N);
    //printf("%x %llx\n", plain_text[i], ct[i]);
    printf("%lld ", ct[i]);
  }
}

void decrypt(){
  cout << "\n";
  for(int i = 0 ; i < len; i++){
    deciphered_text[i] = my_modulus(ct[i], private_key, N);
  }
  printf("\nDeciphered plaintext is \"%s\"\n", deciphered_text);

}

int main(){

  long long p;
  long long q;

  while(1){
    cout << "Enter p : ";
    cin >> p;
    cout << "Enter q : ";
    cin >> q;

    if(are_primes(p, q))
      break;
    else{
      cout << "p or q is not prime\n";
    }
  }

  key_generation(p, q);

  // obtain plain text that is less than 20 chars
  cout << "Enter a plaintext : ";
  cin.ignore();
  cin.getline(plain_text, 20, '\n');
  printf("\nYou entered \"%s\"\n", plain_text);

  encrypt();

  decrypt();
 

  return 0;
}
