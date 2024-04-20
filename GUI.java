import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JButton;
import javax.swing.JTextArea;
import javax.swing.JScrollPane;
import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.util.Scanner;

public class SimpleGUI extends JFrame {

  private JButton openButton;
  private JTextArea textArea;

  public SimpleGUI() {

    // Configurer la fenêtre
    setTitle("Simple GUI");
    setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    setLayout(new FlowLayout());

    // Créer les composants
    openButton = new JButton("Ouvrir fichier"); 
    textArea = new JTextArea(10, 40);
    textArea.setEditable(false);

    // Ajouter les composants
    add(openButton);
    add(new JScrollPane(textArea));

    // Ajouter les listeners
    openButton.addActionListener(new OpenButtonListener());

    // Rendre la fenêtre visible
    setVisible(true);
    pack();
  }

  class OpenButtonListener implements ActionListener {

    @Override
    public void actionPerformed(ActionEvent e) {

      // Ouvrir un JFileChooser
      JFileChooser fileChooser = new JFileChooser();
      if (fileChooser.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
        
        // Récupérer le fichier choisi
        File file = fileChooser.getSelectedFile();
        
        // Lire et afficher le contenu
        displayFile(file); 
      }

    }

    public void displayFile(File file) {
     
      textArea.setText("");
      
      try (Scanner scanner = new Scanner(file)) {
        while (scanner.hasNextLine()) {
          String line = scanner.nextLine();
          textArea.append(line + "\n"); 
        }
      } catch (Exception ex) {
        textArea.setText("Erreur lecture fichier");
      }
    }

  }

  public static void main(String[] args) {
    new SimpleGUI(); 
  }

}

