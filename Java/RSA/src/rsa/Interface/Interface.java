package rsa.Interface;

import java.math.BigInteger;
import rsa.RSA;

public class Interface extends javax.swing.JPanel {
    public Interface() {
        initComponents();
        
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jLabel1 = new javax.swing.JLabel();
        txtN3 = new javax.swing.JTextField();
        txtD = new javax.swing.JTextField();
        jScrollPane1 = new javax.swing.JScrollPane();
        txtCrypto = new javax.swing.JTextArea();
        jLabel2 = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        btnGenerer = new javax.swing.JButton();
        jScrollPane2 = new javax.swing.JScrollPane();
        txtM = new javax.swing.JTextArea();

        jLabel1.setText("Message :");

        txtN3.setEnabled(false);

        txtD.setEnabled(false);

        txtCrypto.setColumns(20);
        txtCrypto.setRows(5);
        txtCrypto.setEnabled(false);
        jScrollPane1.setViewportView(txtCrypto);

        jLabel2.setText("n");

        jLabel3.setText("d");

        jLabel4.setText("Cryptogramme");

        btnGenerer.setText("Décrypter");
        btnGenerer.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnGenererActionPerformed(evt);
            }
        });

        txtM.setColumns(20);
        txtM.setRows(5);
        jScrollPane2.setViewportView(txtM);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(44, 44, 44)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jLabel4)
                            .addComponent(jLabel3, javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(jLabel2, javax.swing.GroupLayout.Alignment.TRAILING))
                        .addGap(18, 18, 18)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(txtN3)
                            .addComponent(txtD)
                            .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 256, Short.MAX_VALUE)))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addComponent(jLabel1)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(layout.createSequentialGroup()
                                .addGap(10, 10, 10)
                                .addComponent(btnGenerer, javax.swing.GroupLayout.PREFERRED_SIZE, 187, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 256, javax.swing.GroupLayout.PREFERRED_SIZE))))
                .addContainerGap(61, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(24, 24, 24)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 67, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel1))
                .addGap(11, 11, 11)
                .addComponent(btnGenerer, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txtN3, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel2))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txtD, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel3))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 57, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel4))
                .addContainerGap(21, Short.MAX_VALUE))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void btnGenererActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnGenererActionPerformed
        RSA rsa = new RSA();
        BigInteger p = rsa.genererNombrePremier();
        BigInteger q = rsa.genererNombrePremier();
        BigInteger n = rsa.calculerN(p, q);
        BigInteger phi = rsa.calculerPhi(p, q);
        BigInteger e = rsa.calculerE(phi);
        BigInteger d = e.modInverse(phi);
        int l = rsa.longueur_bloc(n);
        
        String Message = txtM.getText();
        String Crypto = rsa.encoderRSA(Message, n, e);
        String Decrypto = rsa.decoderRSA(Crypto, n, d);
        
        txtN3.setText(e.toString());
        txtD.setText(n.toString());
        txtCrypto.setText(Crypto);
        
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
    }//GEN-LAST:event_btnGenererActionPerformed


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton btnGenerer;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JTextArea txtCrypto;
    private javax.swing.JTextField txtD;
    private javax.swing.JTextArea txtM;
    private javax.swing.JTextField txtN3;
    // End of variables declaration//GEN-END:variables
}
