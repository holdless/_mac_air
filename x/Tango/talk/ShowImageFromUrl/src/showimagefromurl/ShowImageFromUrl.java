/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package showimagefromurl;

import java.awt.BorderLayout;
import java.awt.Image;
import java.io.IOException;
import java.net.URL;
import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

/**
 *
 * @author hiroshi
 */
public class ShowImageFromUrl {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        Image image = null;
        try {
            URL url = new URL(
                    "http://r.ddmcdn.com/s_f/o_1/cx_462/cy_245/cw_1349/ch_1349/w_720/APL/uploads/2015/06/caturday-shutterstock_149320799.jpg");
            image = ImageIO.read(url);
        } 
        catch (IOException e) {
        }

// Use a label to display the image
        JFrame frame = new JFrame();

        JLabel lblimage = new JLabel(new ImageIcon(image));
        frame.getContentPane().add(lblimage, BorderLayout.CENTER);
        frame.setSize(300, 400);
        frame.setVisible(true);
        
        JPanel mainPanel = new JPanel(new BorderLayout());
        mainPanel.add(lblimage);
// add more components here
        frame.add(mainPanel);
        frame.setVisible(true);
    }

}
