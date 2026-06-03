#include <iostream>
#include <string>
#include <emscripten/bind.h>
//=========================================================
class CaesarCipher
{
 private:
  int shift;
 public:
  CaesarCipher(int shiftValue)
  {shift=shiftValue;}
  std::string encrypt(std::string message)
  {
   std::string result="";
   for(char letter:message)
   {
    if(isupper(letter))
    {result+=((letter-65)+shift)%26+65;}
    else if(islower(letter))
    {result+=((letter-97)+shift)%26+97;}
    else
    {result+=letter;}
   }
   return result;
  }
  std::string decrypt(std::string message)
  {
   std::string result="";
   for(char letter:message)
   {
    if(isupper(letter))
    {result+=((letter-65)-shift+26)%26+65;}
    else if(islower(letter))
    {result+=((letter-97)-shift+26)%26+97;}
    else
    {result+=letter;}
   }
   return result;
  }
};
//=========================================================
class AffineCipher
{
 private:
  int keyA;int keyB;
  int modInverse(int a,int m)
  {
   for(int inv=1;inv<m;inv++)
   {
    if((a*inv)%m==1)
    {return inv;}
   }
   return -1;
  }
 public:
  AffineCipher(int a,int b)
  {keyA=a;keyB=b;}
  std::string encrypt(std::string message)
  {
   std::string result="";
   for(char letter:message)
   {
    if(isupper(letter))
    {result+=(keyA*(letter-65)+keyB)%26+65;}
    else if(islower(letter))
    {result+=(keyA*(letter-97)+keyB)%26+97;}
    else
    {result+=letter;}
   }
   return result;
  }
  std::string decrypt(std::string message)
  {
   int inv=modInverse(keyA,26);
   std::string result="";
   for(char letter:message)
   {
    if(isupper(letter))
    {
     int x=letter-65;
     int decrypted=(inv*(x-keyB+26))%26;
     result+=decrypted+65;
    }
    else if(islower(letter))
    {
     int x=letter-97;
     int decrypted=(inv*(x-keyB+26))%26;
     result+=decrypted+97;
    }
    else
    {result+=letter;}
   }
   return result;
  }
};
//=========================================================
class VigenereCipher
{
 private:
  std::string key;
 public:
  VigenereCipher(std::string k){key=k;}
  std::string encrypt(std::string message)
  {
   std::string result="";int keyIndex=0;
   for(char letter:message)
   {
    if(isalpha(letter))
    {
     char currentKeyChar=toupper(key[keyIndex]);
     int shiftAmount=currentKeyChar-'A';
     if(isupper(letter))
     {result+=((letter-65)+shiftAmount)%26+65;}
     else if(islower(letter))
     {result+=((letter-97)+shiftAmount)%26+97;}
     keyIndex=(keyIndex+1)%key.length();
    }
    else
    {result+=letter;}
   }
   return result;
  }
  std::string decrypt(std::string message)
  {
   std::string result="";
   int keyIndex=0;
   for(char letter:message)
   {
    if(isalpha(letter))
    {
     char currentKeyChar=toupper(key[keyIndex]);
     int shiftAmount=currentKeyChar-'A';
     if(isupper(letter))
     {result+=((letter-65)-shiftAmount+26)%26+65;}
     else if(islower(letter))
     {result+=((letter-97)-shiftAmount+26)%26+97;}
     keyIndex=(keyIndex+1)%key.length();
    }
    else
    {result+=letter;}
   }
   return result;
  }
};
//=========================================================
class PlayfairCipher
{
 private:
  char grid[5][5];
  void findPos(char c,int &row,int &col)
  {
   if(c=='J')c='I';
   for(int r=0;r<5;r++)
   {
    for(int k=0;k<5;k++)
    {
     if(grid[r][k]==c){row=r;col=k;return;}
    }
   }
  }
 public:
  PlayfairCipher(std::string keyString)
  {
   std::string alphabet="ABCDEFGHIKLMNOPQRSTUVWXYZ";
   std::string cleanKey="";
   for(char c:keyString)
   {
    c=toupper(c);
    if(c=='J')c='I';
    if(isalpha(c)&&cleanKey.find(c)==std::string::npos)cleanKey+=c;
   }
   for(char c:alphabet)
   {if(cleanKey.find(c)==std::string::npos)cleanKey+=c;}
   int idx=0;
   for(int r=0;r<5;r++)
   {for(int c=0;c<5;c++)grid[r][c]=cleanKey[idx++];}
  }
  std::string encrypt(std::string message)
  {
   std::string clean="";
   for(char c:message)if(isalpha(c))clean+=toupper(c);
   std::string pairs="";
   for(size_t i=0;i<clean.length();i++)
   {
    pairs+=clean[i];
    if(i+1<clean.length()&&clean[i]==clean[i+1])pairs+='X';
    else if(i+1<clean.length())pairs+=clean[++i];
   }
   if(pairs.length()%2!=0)pairs+='X';
   std::string result="";
   for(size_t i=0;i<pairs.length();i+=2)
   {
    int r1,c1,r2,c2;
    findPos(pairs[i],r1,c1);
    findPos(pairs[i+1],r2,c2);
    if(r1==r2)
    {
     result+=grid[r1][(c1+1)%5];
     result+=grid[r2][(c2+1)%5];
    }
    else if(c1==c2)
    {
     result+=grid[(r1+1)%5][c1];
     result+=grid[(r2+1)%5][c2];
    }
    else
    {
     result+=grid[r1][c2];
     result+=grid[r2][c1];
    }
   }
   return result;
  }
  std::string decrypt(std::string message)
  {
   std::string clean="";
   for(char c:message)if(isalpha(c))clean+=toupper(c);
   std::string pairs="";
   for(size_t i=0;i<clean.length();i+=2)
   {pairs+=clean[i];pairs+=clean[i+1];}
   std::string result="";
   for(size_t i=0;i<pairs.length();i+=2)
   {
    int r1,c1,r2,c2;
    findPos(pairs[i],r1,c1);
    findPos(pairs[i+1],r2,c2);
    if(r1==r2)
    {
     result+=grid[r1][(c1-1+5)%5];
     result+=grid[r2][(c2-1+5)%5];
    }
    else if(c1==c2)
    {
     result+=grid[(r1-1+5)%5][c1];
     result+=grid[(r2-1+5)%5][c2];
    }
    else
    {
     result+=grid[r1][c2];
     result+=grid[r2][c1];
    }
   }
   return result;
  }
};
//=========================================================
EMSCRIPTEN_BINDINGS(cipher_module)
{
  emscripten::class_<CaesarCipher>("CaesarCipher")
    .constructor<int>()
    .function("encrypt",&CaesarCipher::encrypt)
    .function("decrypt",&CaesarCipher::decrypt);
  emscripten::class_<AffineCipher>("AffineCipher")
    .constructor<int,int>()
    .function("encrypt",&AffineCipher::encrypt)
    .function("decrypt",&AffineCipher::decrypt);
  emscripten::class_<VigenereCipher>("VigenereCipher")
    .constructor<std::string>()
    .function("encrypt",&VigenereCipher::encrypt)
    .function("decrypt",&VigenereCipher::decrypt);
  emscripten::class_<PlayfairCipher>("PlayfairCipher")
    .constructor<std::string>()
    .function("encrypt",&PlayfairCipher::encrypt)
    .function("decrypt",&PlayfairCipher::decrypt);
}