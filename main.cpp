#include <iostream>
#include <string>
#include "Matrix.h"
#include "Image.h"
using namespace std;
void mainmenu(int &choice) {
    cout << "\n--- Image Processing Menu ---\n";
    cout << "1. Load Image\n";
    cout << "2. Save Image\n"; 
    cout << "3. Basic Image Operations\n";
    cout << "4. Advanced Image Operations(Compression/Decompression)\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;
}
void basicmenu(int &choice) {
    cout << "\n--- Basic Image Operations Menu ---\n";
    cout << "1. Negative\n";
    cout << "2. Adjust Brightness\n";
    cout << "3. Adjust Contrast\n";
    cout << "4. Threshold\n";
    cout << "5. Grayscale\n";
    cout << "6. Scaling\n";
    cout << "7. Sharpen\n";
    cout << "8. Sobel Edging\n";
    cout << "9. Flip Horizontal\n";
    cout << "10. Flip Vertical\n";
    cout << "11. Exit to Main Menu\n";
    cout << "Enter your choice: ";
    cin >> choice;
}
void advancedmenu(int& choice) {
    cout << "\n--- Advanced Image Operations Menu ---\n";
    cout << "1. Save Compression\n";
    cout << "2. Load Compression\n";
    cout << "3. Save Reconstructed\n";
    cout << "4. Show Compression Info\n";
    cout << "5. Exit to Main Menu\n";
    cout << "Enter your choice: ";
    cin >> choice;
}
int main() {
    Image<unsigned char> img;
    string filename;
    string loadname;
    int value;
    int choice;
   unsigned char thresh;

    do {
        mainmenu(choice);
        switch (choice) {
        case 1:   
			cout << "Enter filename to load: ";
			cin >> loadname;
            if (img.load(loadname))
                cout << "Image loaded successfully!\n";
            else
                cout << "Failed to load image.\n";
            break;
        case 2:
            cout << "Enter filename to save: ";
            cin >> filename;
            if (img.save(filename))
                cout << "Image saved successfully!\n";
            else
                cout << "Failed to save image.\n";
            break;
        case 3:
            int ch2;
			basicmenu(ch2);
            switch (ch2) {
            case 1:
                img.negative();
                cout << "Negative applied!\n";
                break;
            case 2:
                cout << "Enter brightness adjustment value (positive or negative): ";
                cin >> value;
                img.adjustBrightness(value);
                cout << "Brightness adjusted!\n";
                break;
            case 3:
                cout << "Enter Contrast value (0-255): ";
                cin >> value;
                img.adjustContrast((unsigned char)(value));
                cout << "Contrast applied!\n";
                break;
            case 4:
                cout << "Enter threshold value (0-255): ";
                cin >> value;
                thresh = (unsigned char)(value);
                img.threshold(thresh);
                cout << "Threshold applied!\n";
                break;
            case 5:
                img.Grayscale();
                cout << "Grayscale applied!\n";
                break;
            case 6:
                int newWidth, newHeight;
                cout << "Enter new width: ";
                cin >> newWidth;
                cout << "Enter new height: ";
                cin >> newHeight;
                img.scaling(newWidth, newHeight);
                cout << "Scaling applied!\n";
                break;
            case 7:
                img.sharpen();
                cout << "Sharpen applied!\n";
                break;
            case 8:
                img.sobelEdgeDetection();
                cout << "Sobel Edge Detection applied!\n";
                break;
            case 9:
                img.flipHorizontal();
                cout << "Flip Horizontal applied!\n";
                break;
            case 10:
                img.flipVertical();
                cout << "Flip Vertical applied!\n";
                break;
            case 11:
                cout<<"Exiting Basic Operations\n";
                break;
			default:
				cout << "Invalid choice. Try again.\n";
            }
        case 4:
            int ch3;
			advancedmenu(ch3);			
            switch (ch3) {
            case 1:
                cout << "Enter filename to save compressed image: ";
                cin >> filename;
                if (img.saveCompressed(filename))
                    cout << "Compressed image saved successfully!\n";
                else
                    cout << "Failed to save compressed image.\n";
                break;
            case 2:
                cout << "Enter filename to load compressed image: ";
                cin >> filename;
                if (img.loadCompressed(filename))
                    cout << "Compressed image loaded successfully!\n";
                else
                    cout << "Failed to load compressed image.\n";
                break;
            case 3:
                cout << "Enter filename to save reconstructed image: ";
                cin >> filename;
                if (img.save(filename))
                    cout << "Reconstructed image saved successfully!\n";
                else
                    cout << "Failed to save reconstructed image.\n";
                break;
            case 4:
                cout << "Original Size: " << img.originalSize() << " bytes\n";
                cout << "Compressed Size: " << img.compressedSize() << " bytes\n";
                cout << "Compression Rate: " << img.compressionRate() << "%\n";
            case 5:
                cout << "Exiting\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
            }    
        }
    } while (choice != 5);
    return 0;   
}