package rsa;

import java.math.BigInteger;
import java.util.Random;

public class RSA {
    public static int longeur_defaut = 10;
    
    public static BigInteger genererPrime(int longueur_en_bits)
    {
        return BigInteger.probablePrime(longueur_en_bits, new Random());
    }
    
    public BigInteger genererNombrePremier()
    {
        return BigInteger.probablePrime(RSA.longeur_defaut, new Random());
    }
    
    public BigInteger calculerN(BigInteger p, BigInteger q)
    {
        return p.multiply(q);
    }
    
    public BigInteger calculerPhi(BigInteger p, BigInteger q)
    {
        return p.add(BigInteger.ONE.negate()).multiply(q.add(BigInteger.ONE.negate()));
    }
    
    public BigInteger calculerE(BigInteger phi)
    {
        BigInteger e = genererPrime(2);
        
        while(!phi.gcd(e).equals(BigInteger.ONE))
            e = e.add(BigInteger.ONE);
        
        return e;
    }
    
    public int longueur_bloc(BigInteger n)
    {
        return n.toString().length();
    }
    
    public String encoderRSA(String Message, BigInteger n, BigInteger e)
    {
        String Crypto = "";
        int l = longueur_bloc(n);
        
        // besoin de e et n
        for(char m : Message.toCharArray())
        {
            String M = "" + (int)m;
            BigInteger c = new BigInteger(M).modPow(e, n);
            for(int i = 0; i < l - c.toString().length(); i++)
                Crypto += "0";
            Crypto += c;
        }
        
        return Crypto;
    }
    
    public String decoderRSA(String Crypto, BigInteger n, BigInteger d)
    {
        String Decrypto = "";
        int l = longueur_bloc(n);
        // Besoin de n, d et l
        for(int i = 0; i < Crypto.length()/l; i++)
        {
            String nieme_char = Crypto.substring(i*l, (i+1)*l);
            BigInteger c = new BigInteger(nieme_char).modPow(d, n);
            Decrypto += ((char) c.intValue()) + " ";
        }
        
        return Decrypto;
    }
    
    public static void main(String[] args) {
        RSA rsa = new RSA();
        BigInteger p = rsa.genererNombrePremier();
        BigInteger q = rsa.genererNombrePremier();
        BigInteger n = rsa.calculerN(p, q);
        BigInteger phi = rsa.calculerPhi(p, q);
        BigInteger e = rsa.calculerE(phi);
        BigInteger d = e.modInverse(phi);
        int l = rsa.longueur_bloc(n);
        
        String Message = "Salut";
        String Crypto = rsa.encoderRSA(Message, n, e);
        String Decrypto = rsa.decoderRSA(Crypto, n, d);
        
        System.out.println("p   : " + p);
        System.out.println("q   : " + q);
        System.out.println("n   : " + n);
        System.out.println("phi : " + phi);
        System.out.println("e   : " + e);
        System.out.println("d   : " + d);
        System.out.println("l   : " + l);
        System.out.println("Message Original : " + Message);
        System.out.println("Message Crypté   : " + Crypto);
        System.out.println("Message Décrypté : " + Decrypto);
    }
    
}
