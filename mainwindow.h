#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QStackedWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include "Image.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Main Menu Functions
    void loadImage();
    void showBasicMenu();
    void showAdvancedMenu();
    void exitApplication();

    // Basic Operations
    void applyNegative();
    void applyGrayscale();
    void applyThreshold();
    void adjustBrightness();
    void adjustContrast();
    void scaleImage();
    void applySharpen();
    void applySobel();
    void flipHorizontal();
    void flipVertical();
    void clearOperations();
    void backToMainMenu();

    // Advanced Operations
    void saveCompressed();
    void loadCompressed();
    void showCompressionInfo();
    void saveImage();
    void backToMainFromAdvanced();

private:
    void createUI();
    void createMainMenu();
    void createBasicMenu();
    void createAdvancedMenu();
    void updateImageDisplay();
    void setStatus(const QString& message, bool isError = false);
    QPixmap convertImageToQPixmap();

    // Helper functions
    QPushButton* createNavButton(const QString& text, const QString& color);
    QPushButton* createOpButton(const QString& text, const QString& color);

    // UI Components
    QStackedWidget* stackedWidget;
    QWidget* mainMenuWidget;
    QWidget* basicMenuWidget;
    QWidget* advancedMenuWidget;
    QLabel* imageLabel;
    QLabel* mainImageLabel;  // Add this with the other QLabel declarations
    QLabel* statusLabel;
    QLabel* imageInfoLabel;

    // Input fields
    QLineEdit* thresholdEdit;
    QLineEdit* brightnessEdit;
    QLineEdit* contrastEdit;
    QLineEdit* widthEdit;
    QLineEdit* heightEdit;

    // Data
    Image<unsigned char> currentImage;
    Image<unsigned char> originalImage;
    QString currentImagePath;
    bool imageLoaded;
    QString lastSavePath;
};

#endif // MAINWINDOW_H