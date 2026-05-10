#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QGridLayout>
#include <QFrame>
#include <QTimer>
#include <QFileInfo>

// Global pointers for image labels
static QLabel* g_basicImageLabel = nullptr;
static QLabel* g_advancedImageLabel = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , imageLoaded(false)
{
    createUI();
    setWindowTitle("Image Processor - DSA Project");
    resize(1300, 800);
    setMinimumSize(1000, 600);
    setStatus("Welcome! Please load an image to begin.");
}

MainWindow::~MainWindow() {}

QPushButton* MainWindow::createNavButton(const QString& text, const QString& color) {
    QPushButton* btn = new QPushButton(text);
    btn->setStyleSheet(QString(
                           "QPushButton { "
                           "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 %1, stop:1 %2); "
                           "color: white; "
                           "font-size: 12pt; "
                           "font-weight: bold; "
                           "padding: 12px; "
                           "border: none; "
                           "border-radius: 8px; "
                           "margin: 4px; "
                           "}"
                           "QPushButton:hover { "
                           "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 %2, stop:1 %3); "
                           "}"
                           "QPushButton:pressed { "
                           "padding-top: 14px; "
                           "padding-bottom: 10px; "
                           "}"
                           ).arg(color, color + "aa", color + "cc"));
    return btn;
}

QPushButton* MainWindow::createOpButton(const QString& text, const QString& color) {
    QPushButton* btn = new QPushButton(text);
    btn->setMinimumHeight(40);  // Add this - ensures button text is fully visible
    btn->setStyleSheet(QString(
                           "QPushButton { "
                           "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 %1, stop:1 %2); "
                           "color: white; "
                           "font-size: 10pt; "
                           "font-weight: bold; "
                           "padding: 8px 5px; "
                           "border: none; "
                           "border-radius: 6px; "
                           "}"
                           "QPushButton:hover { "
                           "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 %2, stop:1 %3); "
                           "}"
                           "QPushButton:pressed { "
                           "padding-top: 10px; "
                           "padding-bottom: 6px; "
                           "}"
                           ).arg(color, color + "aa", color + "cc"));
    return btn;
}

void MainWindow::createUI() {
    // Central widget with gradient background
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setStyleSheet(
        "QWidget { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #1a1a2e, stop:0.5 #16213e, stop:1 #0f3460); "
        "}"
        );
    setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // ==================== LEFT SIDEBAR ====================
    QWidget* sidebar = new QWidget();
    sidebar->setFixedWidth(280);
    sidebar->setStyleSheet(
        "QWidget { "
        "background: rgba(30, 30, 46, 0.95); "
        "border-radius: 15px; "
        "border: 1px solid rgba(255, 255, 255, 0.1); "
        "}"
        );

    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setSpacing(10);
    sidebarLayout->setContentsMargins(15, 25, 15, 25);

    QLabel* titleLabel = new QLabel("🖼️ Image\nProcessor");
    titleLabel->setStyleSheet(
        "color: #e94560; "
        "font-size: 22pt; "
        "font-weight: bold; "
        "padding: 20px 0; "
        "background: transparent; "
        );
    titleLabel->setAlignment(Qt::AlignCenter);
    sidebarLayout->addWidget(titleLabel);

    QPushButton* loadBtn = createNavButton("📂 LOAD IMAGE", "#e94560");
    QPushButton* basicBtn = createNavButton("🎨 BASIC OPS", "#533483");
    QPushButton* advancedBtn = createNavButton("🔧 ADVANCED OPS", "#0f3460");
    QPushButton* exitBtn = createNavButton("🚪 EXIT", "#c21010");

    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::loadImage);
    connect(basicBtn, &QPushButton::clicked, this, &MainWindow::showBasicMenu);
    connect(advancedBtn, &QPushButton::clicked, this, &MainWindow::showAdvancedMenu);
    connect(exitBtn, &QPushButton::clicked, this, &MainWindow::exitApplication);

    sidebarLayout->addWidget(loadBtn);
    sidebarLayout->addWidget(basicBtn);
    sidebarLayout->addWidget(advancedBtn);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(exitBtn);

    imageInfoLabel = new QLabel();
    imageInfoLabel->setStyleSheet(
        "color: #e0e0e0; "
        "font-size: 10pt; "
        "padding: 12px; "
        "background: rgba(0, 0, 0, 0.3); "
        "border-radius: 8px; "
        "margin-top: 20px; "
        );
    imageInfoLabel->setWordWrap(true);
    imageInfoLabel->setAlignment(Qt::AlignCenter);
    imageInfoLabel->setText("No Image Loaded");
    sidebarLayout->addWidget(imageInfoLabel);

    // ==================== RIGHT AREA ====================
    QWidget* rightArea = new QWidget();
    rightArea->setStyleSheet(
        "QWidget { "
        "background: rgba(255, 255, 255, 0.05); "
        "border-radius: 15px; "
        "}"
        );
    QVBoxLayout* rightLayout = new QVBoxLayout(rightArea);
    rightLayout->setContentsMargins(20, 20, 20, 20);

    stackedWidget = new QStackedWidget();
    stackedWidget->setStyleSheet("QStackedWidget { background: transparent; }");

    createMainMenu();
    createBasicMenu();
    createAdvancedMenu();

    stackedWidget->addWidget(mainMenuWidget);
    stackedWidget->addWidget(basicMenuWidget);
    stackedWidget->addWidget(advancedMenuWidget);

    rightLayout->addWidget(stackedWidget);

    statusLabel = new QLabel();
    statusLabel->setStyleSheet(
        "QLabel { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #533483, stop:1 #e94560); "
        "color: white; "
        "padding: 10px; "
        "font-size: 10pt; "
        "border-radius: 8px; "
        "font-weight: bold; "
        "}"
        );
    statusLabel->setMaximumHeight(40);
    rightLayout->addWidget(statusLabel);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(rightArea, 1);
}

void MainWindow::createMainMenu() {
    mainMenuWidget = new QWidget();
    mainMenuWidget->setStyleSheet("QWidget { background: transparent; }");
    QVBoxLayout* layout = new QVBoxLayout(mainMenuWidget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* welcomeLabel = new QLabel();
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet(
        "QLabel { "
        "font-size: 24pt; "
        "font-weight: bold; "
        "color: #e94560; "
        "background: transparent; "
        "}"
        );
    welcomeLabel->setText("🎯 IMAGE PROCESSING SYSTEM\n✨ DSA Project ✨");
    layout->addWidget(welcomeLabel);

    // ========== IMAGE PREVIEW IN MAIN MENU ==========
    QWidget* imageContainer = new QWidget();
    imageContainer->setFixedHeight(350);
    imageContainer->setStyleSheet(
        "QWidget { "
        "background: rgba(0, 0, 0, 0.3); "
        "border: 2px solid rgba(255, 255, 255, 0.1); "
        "border-radius: 15px; "
        "}"
        );

    QVBoxLayout* containerLayout = new QVBoxLayout(imageContainer);
    containerLayout->setAlignment(Qt::AlignCenter);
    containerLayout->setContentsMargins(5, 5, 5, 5);

    mainImageLabel = new QLabel();
    mainImageLabel->setAlignment(Qt::AlignCenter);
    mainImageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainImageLabel->setStyleSheet(
        "QLabel { "
        "background: rgba(0, 0, 0, 0.2); "
        "color: #a0a0a0; "
        "font-size: 14pt; "
        "border-radius: 10px; "
        "}"
        );
    mainImageLabel->setText("No Image Loaded\n\n🌸 Click 'LOAD IMAGE' to begin 🌸");
    containerLayout->addWidget(mainImageLabel);

    layout->addWidget(imageContainer);
    layout->addStretch();
}

void MainWindow::createBasicMenu() {
    basicMenuWidget = new QWidget();
    basicMenuWidget->setStyleSheet("QWidget { background: transparent; }");
    QVBoxLayout* mainLayout = new QVBoxLayout(basicMenuWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    QLabel* title = new QLabel("🎨 BASIC OPERATIONS");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "QLabel { "
        "font-size: 18pt; "
        "font-weight: bold; "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #e94560, stop:1 #533483); "
        "padding: 10px; "
        "border-radius: 10px; "
        "color: white; "
        "}"
        );
    mainLayout->addWidget(title);

    // Image preview container
    QWidget* imageContainer = new QWidget();
    imageContainer->setFixedHeight(300);
    imageContainer->setStyleSheet(
        "QWidget { "
        "background: rgba(0, 0, 0, 0.3); "
        "border: 2px solid rgba(255, 255, 255, 0.1); "
        "border-radius: 15px; "
        "}"
        );

    QVBoxLayout* containerLayout = new QVBoxLayout(imageContainer);
    containerLayout->setAlignment(Qt::AlignCenter);
    containerLayout->setContentsMargins(5, 5, 5, 5);

    g_basicImageLabel = new QLabel();
    g_basicImageLabel->setAlignment(Qt::AlignCenter);
    g_basicImageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    g_basicImageLabel->setStyleSheet(
        "QLabel { "
        "background: rgba(0, 0, 0, 0.2); "
        "color: #a0a0a0; "
        "border-radius: 10px; "
        "}"
        );
    g_basicImageLabel->setText("No Image Loaded\n\n✨ Please load an image first ✨");
    containerLayout->addWidget(g_basicImageLabel);

    mainLayout->addWidget(imageContainer);

    // Buttons Grid - with proper spacing for full text
    QGridLayout* opsLayout = new QGridLayout();
    opsLayout->setSpacing(10);
    opsLayout->setContentsMargins(5, 5, 5, 5);

    QPushButton* negativeBtn = createOpButton("🎨 Negative", "#e74c3c");
    QPushButton* grayscaleBtn = createOpButton("⚫ Grayscale", "#7f8c8d");
    QPushButton* thresholdBtn = createOpButton("⚪ Threshold", "#95a5a6");
    QPushButton* brightnessBtn = createOpButton("☀️ Brightness", "#f39c12");
    QPushButton* contrastBtn = createOpButton("🌓 Contrast", "#f1c40f");
    QPushButton* flipHBtn = createOpButton("🔄 Flip Horizontal", "#1abc9c");
    QPushButton* flipVBtn = createOpButton("🔄 Flip Vertical", "#1abc9c");
    QPushButton* sharpenBtn = createOpButton("🔪 Sharpen", "#3498db");
    QPushButton* sobelBtn = createOpButton("🔍 Sobel Edge", "#9b59b6");
    QPushButton* scaleBtn = createOpButton("📐 Scale", "#16a085");
    QPushButton* clearBtn = createOpButton("🔄 CLEAR OPS", "#e67e22");
    QPushButton* saveBtn = createOpButton("💾 SAVE IMAGE", "#27ae60");
    QPushButton* backBtn = createOpButton("◀ BACK TO MAIN", "#2c3e50");

    connect(negativeBtn, &QPushButton::clicked, this, &MainWindow::applyNegative);
    connect(grayscaleBtn, &QPushButton::clicked, this, &MainWindow::applyGrayscale);
    connect(thresholdBtn, &QPushButton::clicked, this, &MainWindow::applyThreshold);
    connect(brightnessBtn, &QPushButton::clicked, this, &MainWindow::adjustBrightness);
    connect(contrastBtn, &QPushButton::clicked, this, &MainWindow::adjustContrast);
    connect(flipHBtn, &QPushButton::clicked, this, &MainWindow::flipHorizontal);
    connect(flipVBtn, &QPushButton::clicked, this, &MainWindow::flipVertical);
    connect(sharpenBtn, &QPushButton::clicked, this, &MainWindow::applySharpen);
    connect(sobelBtn, &QPushButton::clicked, this, &MainWindow::applySobel);
    connect(scaleBtn, &QPushButton::clicked, this, &MainWindow::scaleImage);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::clearOperations);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::saveImage);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::backToMainMenu);

    opsLayout->addWidget(negativeBtn, 0, 0);
    opsLayout->addWidget(grayscaleBtn, 0, 1);
    opsLayout->addWidget(thresholdBtn, 0, 2);
    opsLayout->addWidget(brightnessBtn, 1, 0);
    opsLayout->addWidget(contrastBtn, 1, 1);
    opsLayout->addWidget(flipHBtn, 1, 2);
    opsLayout->addWidget(flipVBtn, 2, 0);
    opsLayout->addWidget(sharpenBtn, 2, 1);
    opsLayout->addWidget(sobelBtn, 2, 2);
    opsLayout->addWidget(scaleBtn, 3, 0);
    opsLayout->addWidget(clearBtn, 3, 1);
    opsLayout->addWidget(saveBtn, 3, 2);
    opsLayout->addWidget(backBtn, 4, 0, 1, 3);

    mainLayout->addLayout(opsLayout);

    // Input fields
    QHBoxLayout* inputLayout = new QHBoxLayout();
    inputLayout->setSpacing(10);

    QString groupStyle =
        "QGroupBox { "
        "color: #e0e0e0; "
        "font-weight: bold; "
        "font-size: 9pt; "
        "border: 1px solid rgba(255, 255, 255, 0.1); "
        "border-radius: 6px; "
        "margin-top: 8px; "
        "padding-top: 8px; "
        "}"
        "QGroupBox::title { "
        "subcontrol-origin: margin; "
        "left: 8px; "
        "padding: 0 3px 0 3px; "
        "color: #e94560; "
        "}"
        "QLineEdit { "
        "background: rgba(0, 0, 0, 0.5); "
        "color: white; "
        "border: 1px solid rgba(255, 255, 255, 0.2); "
        "border-radius: 4px; "
        "padding: 3px; "
        "font-size: 9pt; "
        "}"
        "QLineEdit:focus { "
        "border: 1px solid #e94560; "
        "}";

    QGroupBox* threshGroup = new QGroupBox("Threshold (0-255)");
    threshGroup->setStyleSheet(groupStyle);
    QHBoxLayout* threshLayout = new QHBoxLayout(threshGroup);
    thresholdEdit = new QLineEdit();
    thresholdEdit->setPlaceholderText("128");
    thresholdEdit->setMaximumWidth(80);
    threshLayout->addWidget(thresholdEdit);

    QGroupBox* brightGroup = new QGroupBox("Brightness (-255-255)");
    brightGroup->setStyleSheet(groupStyle);
    QHBoxLayout* brightLayout = new QHBoxLayout(brightGroup);
    brightnessEdit = new QLineEdit();
    brightnessEdit->setPlaceholderText("50");
    brightnessEdit->setMaximumWidth(80);
    brightLayout->addWidget(brightnessEdit);

    QGroupBox* contrastGroup = new QGroupBox("Contrast (1-5)");
    contrastGroup->setStyleSheet(groupStyle);
    QHBoxLayout* contrastLayout = new QHBoxLayout(contrastGroup);
    contrastEdit = new QLineEdit();
    contrastEdit->setPlaceholderText("2");
    contrastEdit->setMaximumWidth(60);
    contrastLayout->addWidget(contrastEdit);

    QGroupBox* scaleGroup = new QGroupBox("New Size");
    scaleGroup->setStyleSheet(groupStyle);
    QHBoxLayout* scaleLayout = new QHBoxLayout(scaleGroup);
    widthEdit = new QLineEdit();
    widthEdit->setPlaceholderText("W");
    widthEdit->setMaximumWidth(60);
    heightEdit = new QLineEdit();
    heightEdit->setPlaceholderText("H");
    heightEdit->setMaximumWidth(60);
    scaleLayout->addWidget(widthEdit);
    scaleLayout->addWidget(heightEdit);

    inputLayout->addWidget(threshGroup);
    inputLayout->addWidget(brightGroup);
    inputLayout->addWidget(contrastGroup);
    inputLayout->addWidget(scaleGroup);

    mainLayout->addLayout(inputLayout);
}

void MainWindow::createAdvancedMenu() {
    advancedMenuWidget = new QWidget();
    advancedMenuWidget->setStyleSheet("QWidget { background: transparent; }");
    QVBoxLayout* mainLayout = new QVBoxLayout(advancedMenuWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    QLabel* title = new QLabel("🔧 ADVANCED OPERATIONS - RLE COMPRESSION");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "QLabel { "
        "font-size: 16pt; "
        "font-weight: bold; "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #9b59b6, stop:1 #e94560); "
        "padding: 10px; "
        "border-radius: 10px; "
        "color: white; "
        "}"
        );
    mainLayout->addWidget(title);

    // Image preview container
    QWidget* imageContainer = new QWidget();
    imageContainer->setFixedHeight(300);
    imageContainer->setStyleSheet(
        "QWidget { "
        "background: rgba(0, 0, 0, 0.3); "
        "border: 2px solid rgba(255, 255, 255, 0.1); "
        "border-radius: 15px; "
        "}"
        );

    QVBoxLayout* containerLayout = new QVBoxLayout(imageContainer);
    containerLayout->setAlignment(Qt::AlignCenter);
    containerLayout->setContentsMargins(5, 5, 5, 5);

    g_advancedImageLabel = new QLabel();
    g_advancedImageLabel->setAlignment(Qt::AlignCenter);
    g_advancedImageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    g_advancedImageLabel->setStyleSheet(
        "QLabel { "
        "background: rgba(0, 0, 0, 0.2); "
        "color: #a0a0a0; "
        "border-radius: 10px; "
        "}"
        );
    g_advancedImageLabel->setText("No Image Loaded\n\n✨ Load an image to compress ✨");
    containerLayout->addWidget(g_advancedImageLabel);

    mainLayout->addWidget(imageContainer);

    // Buttons
    QGridLayout* btnLayout = new QGridLayout();
    btnLayout->setSpacing(10);

    QPushButton* compressBtn = createOpButton("💾 Save Compressed (.bin)", "#27ae60");
    QPushButton* decompressBtn = createOpButton("📂 Load Compressed (.bin)", "#2980b9");
    QPushButton* clearBtn = createOpButton("🔄 CLEAR OPS", "#e67e22");
    QPushButton* saveImageBtn = createOpButton("💿 Save as PPM", "#f39c12");
    QPushButton* infoBtn = createOpButton("ℹ️ Compression Info", "#8e44ad");
    QPushButton* backBtn = createOpButton("◀ BACK TO MAIN", "#2c3e50");

    connect(compressBtn, &QPushButton::clicked, this, &MainWindow::saveCompressed);
    connect(decompressBtn, &QPushButton::clicked, this, &MainWindow::loadCompressed);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::clearOperations);
    connect(saveImageBtn, &QPushButton::clicked, this, &MainWindow::saveImage);
    connect(infoBtn, &QPushButton::clicked, this, &MainWindow::showCompressionInfo);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::backToMainFromAdvanced);

    btnLayout->addWidget(compressBtn, 0, 0);
    btnLayout->addWidget(decompressBtn, 0, 1);
    btnLayout->addWidget(clearBtn, 1, 0);
    btnLayout->addWidget(saveImageBtn, 1, 1);
    btnLayout->addWidget(infoBtn, 2, 0);
    btnLayout->addWidget(backBtn, 2, 1);

    mainLayout->addLayout(btnLayout);
    mainLayout->addStretch();

    QLabel* infoText = new QLabel(
        "✨ RLE Compression Info ✨\n"
        "• Compresses by storing runs of identical pixels\n"
        "• Best for images with large uniform areas\n"
        "• Saves as .bin binary file format\n"
        "💡 Click 'CLEAR OPS' to reset image"
        );
    infoText->setStyleSheet(
        "QLabel { "
        "background: rgba(0, 0, 0, 0.3); "
        "padding: 8px; "
        "border-radius: 8px; "
        "color: #e0e0e0; "
        "font-size: 9pt; "
        "border: 1px solid rgba(255, 255, 255, 0.1); "
        "}"
        );
    infoText->setWordWrap(true);
    mainLayout->addWidget(infoText);
}

void MainWindow::setStatus(const QString& message, bool isError) {
    if (isError) {
        statusLabel->setStyleSheet(
            "QLabel { "
            "background: #c21010; "
            "color: white; "
            "padding: 10px; "
            "font-size: 10pt; "
            "border-radius: 8px; "
            "font-weight: bold; "
            "}"
            );
    } else {
        statusLabel->setStyleSheet(
            "QLabel { "
            "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
            "stop:0 #533483, stop:1 #e94560); "
            "color: white; "
            "padding: 10px; "
            "font-size: 10pt; "
            "border-radius: 8px; "
            "font-weight: bold; "
            "}"
            );
    }
    statusLabel->setText(message);

    if (!isError) {
        QTimer::singleShot(3000, [this]() {
            statusLabel->setStyleSheet(
                "QLabel { "
                "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
                "stop:0 #533483, stop:1 #e94560); "
                "color: white; "
                "padding: 10px; "
                "font-size: 10pt; "
                "border-radius: 8px; "
                "font-weight: bold; "
                "}"
                );
            statusLabel->setText("Ready");
        });
    }
}

QPixmap MainWindow::convertImageToQPixmap() {
    if (!imageLoaded) return QPixmap();

    int width = currentImage.getWidth();
    int height = currentImage.getHeight();

    if (width <= 0 || height <= 0) return QPixmap();

    QImage qimage(width, height, QImage::Format_RGB888);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pixel p = currentImage.getPixel(x, y);
            qimage.setPixel(x, y, qRgb(p.red, p.green, p.blue));
        }
    }

    return QPixmap::fromImage(qimage);
}

void MainWindow::updateImageDisplay() {
    if (!imageLoaded) {
        if (g_basicImageLabel) {
            g_basicImageLabel->setText("No Image Loaded\n\n✨ Please load an image first ✨");
            g_basicImageLabel->setPixmap(QPixmap());
        }
        if (g_advancedImageLabel) {
            g_advancedImageLabel->setText("No Image Loaded\n\n✨ Load an image to compress ✨");
            g_advancedImageLabel->setPixmap(QPixmap());
        }
        if (mainImageLabel) {
            mainImageLabel->setText("No Image Loaded\n\n🌸 Click 'LOAD IMAGE' to begin 🌸");
            mainImageLabel->setPixmap(QPixmap());
        }
        imageInfoLabel->setText("No Image Loaded");
        return;
    }

    QPixmap pixmap = convertImageToQPixmap();
    if (!pixmap.isNull()) {
        QPixmap scaled = pixmap.scaled(550, 280, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Update Basic Menu image
        if (g_basicImageLabel) {
            g_basicImageLabel->setPixmap(scaled);
            g_basicImageLabel->setScaledContents(false);
            g_basicImageLabel->setAlignment(Qt::AlignCenter);
        }

        // Update Advanced Menu image
        if (g_advancedImageLabel) {
            g_advancedImageLabel->setPixmap(scaled);
            g_advancedImageLabel->setScaledContents(false);
            g_advancedImageLabel->setAlignment(Qt::AlignCenter);
        }

        // Update Main Menu image
        if (mainImageLabel) {
            mainImageLabel->setPixmap(scaled);
            mainImageLabel->setScaledContents(false);
            mainImageLabel->setAlignment(Qt::AlignCenter);
        }

        int width = currentImage.getWidth();
        int height = currentImage.getHeight();
        imageInfoLabel->setText(QString("📷 Image Info:\nSize: %1 x %2\nMemory: ~%3 KB")
                                    .arg(width)
                                    .arg(height)
                                    .arg((width * height * 3) / 1024));
    }
}
// ==================== ALL OPERATION FUNCTIONS ====================

void MainWindow::loadImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Select Image File", "", "PPM Images (*.ppm);;All Files (*)");

    if (fileName.isEmpty()) return;

    if (currentImage.load(fileName.toStdString())) {
        imageLoaded = true;
        currentImagePath = fileName;
        originalImage = currentImage;

        setStatus("✓ Image loaded: " + QFileInfo(fileName).fileName());
        updateImageDisplay();
        stackedWidget->setCurrentWidget(mainMenuWidget);

        QMessageBox::information(this, "Image Loaded",
                                 QString("Image loaded successfully!\n\nFile: %1\nSize: %2 x %3 pixels")
                                     .arg(QFileInfo(fileName).fileName())
                                     .arg(currentImage.getWidth())
                                     .arg(currentImage.getHeight()));
    } else {
        setStatus("✗ Failed to load image. Make sure it's a valid PPM file.", true);
        imageLoaded = false;
    }
}

void MainWindow::showBasicMenu() {
    if (!imageLoaded) {
        setStatus("Please load an image first", true);
        return;
    }
    stackedWidget->setCurrentWidget(basicMenuWidget);
    updateImageDisplay();
}

void MainWindow::showAdvancedMenu() {
    if (!imageLoaded) {
        setStatus("Please load an image first", true);
        return;
    }
    stackedWidget->setCurrentWidget(advancedMenuWidget);
    updateImageDisplay();
}

void MainWindow::exitApplication() {
    close();
}

void MainWindow::backToMainMenu() {
    stackedWidget->setCurrentWidget(mainMenuWidget);
}

void MainWindow::backToMainFromAdvanced() {
    stackedWidget->setCurrentWidget(mainMenuWidget);
}

void MainWindow::clearOperations() {
    if (!imageLoaded) {
        setStatus("No image loaded", true);
        return;
    }

    currentImage = originalImage;
    setStatus("✓ All operations cleared! Image reset to original state");
    updateImageDisplay();
}

void MainWindow::applyNegative() {
    if (!imageLoaded) return;
    currentImage.negative();
    setStatus("✓ Negative effect applied");
    updateImageDisplay();
}

void MainWindow::applyGrayscale() {
    if (!imageLoaded) return;
    currentImage.Grayscale();
    setStatus("✓ Grayscale effect applied");
    updateImageDisplay();
}

void MainWindow::applyThreshold() {
    if (!imageLoaded) return;

    int value = thresholdEdit->text().toInt();
    if (value == 0 && thresholdEdit->text() != "0") {
        bool ok;
        value = QInputDialog::getInt(this, "Threshold", "Enter threshold (0-255):", 128, 0, 255, 1, &ok);
        if (!ok) return;
    }
    if (value < 0) value = 0;
    if (value > 255) value = 255;

    currentImage.threshold(static_cast<unsigned char>(value));
    setStatus(QString("✓ Threshold applied (value: %1)").arg(value));
    updateImageDisplay();
}

void MainWindow::adjustBrightness() {
    if (!imageLoaded) return;

    int value = brightnessEdit->text().toInt();
    if (value == 0 && brightnessEdit->text() != "0") {
        bool ok;
        value = QInputDialog::getInt(this, "Brightness", "Enter brightness (-255 to 255):", 50, -255, 255, 5, &ok);
        if (!ok) return;
    }

    currentImage.adjustBrightness(value);
    setStatus(QString("✓ Brightness adjusted by: %1").arg(value));
    updateImageDisplay();
}

void MainWindow::adjustContrast() {
    if (!imageLoaded) return;

    int value = contrastEdit->text().toInt();
    if (value == 0 && contrastEdit->text() != "0") {
        bool ok;
        value = QInputDialog::getInt(this, "Contrast", "Enter contrast factor (1-5):", 2, 1, 5, 1, &ok);
        if (!ok) return;
    }

    currentImage.adjustContrast(value);
    setStatus(QString("✓ Contrast adjusted (factor: %1)").arg(value));
    updateImageDisplay();
}

void MainWindow::flipHorizontal() {
    if (!imageLoaded) return;
    currentImage.flipHorizontal();
    setStatus("✓ Horizontal flip applied");
    updateImageDisplay();
}

void MainWindow::flipVertical() {
    if (!imageLoaded) return;
    currentImage.flipVertical();
    setStatus("✓ Vertical flip applied");
    updateImageDisplay();
}

void MainWindow::applySharpen() {
    if (!imageLoaded) return;
    currentImage.sharpen();
    setStatus("✓ Sharpen filter applied");
    updateImageDisplay();
}

void MainWindow::applySobel() {
    if (!imageLoaded) return;
    currentImage.sobelEdgeDetection();
    setStatus("✓ Sobel edge detection applied");
    updateImageDisplay();
}

void MainWindow::scaleImage() {
    if (!imageLoaded) return;

    int newWidth = widthEdit->text().toInt();
    int newHeight = heightEdit->text().toInt();

    if (newWidth <= 0 || newHeight <= 0) {
        bool ok;
        newWidth = QInputDialog::getInt(this, "Scale Width", "Enter new width:", 100, 1, 2000, 10, &ok);
        if (!ok) return;
        newHeight = QInputDialog::getInt(this, "Scale Height", "Enter new height:", 100, 1, 2000, 10, &ok);
        if (!ok) return;
    }

    currentImage.scaling(newWidth, newHeight);
    setStatus(QString("✓ Image scaled to %1 x %2").arg(newWidth).arg(newHeight));
    updateImageDisplay();
}

void MainWindow::saveImage() {
    if (!imageLoaded) {
        setStatus("No image to save", true);
        return;
    }

    QString defaultName = QFileInfo(currentImagePath).baseName() + "_processed.ppm";
    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", defaultName, "PPM Images (*.ppm);;All Files (*)");

    if (fileName.isEmpty()) return;

    if (currentImage.save(fileName.toStdString())) {
        setStatus("✓ Image saved: " + QFileInfo(fileName).fileName());
        lastSavePath = fileName;
        QMessageBox::information(this, "Save Successful",
                                 QString("Image saved successfully!\n\nLocation: %1\nSize: %2 x %3 pixels")
                                     .arg(fileName)
                                     .arg(currentImage.getWidth())
                                     .arg(currentImage.getHeight()));
    } else {
        setStatus("✗ Failed to save image", true);
    }
}

void MainWindow::saveCompressed() {
    if (!imageLoaded) {
        setStatus("No image to compress", true);
        return;
    }

    QString defaultName = QFileInfo(currentImagePath).baseName() + ".bin";
    QString fileName = QFileDialog::getSaveFileName(this, "Save Compressed Image", defaultName, "Binary Files (*.bin);;All Files (*)");

    if (fileName.isEmpty()) return;

    if (currentImage.saveCompressed(fileName.toStdString())) {
        setStatus("✓ Image compressed and saved as .bin: " + QFileInfo(fileName).fileName());
        showCompressionInfo();
    } else {
        setStatus("✗ Failed to save compressed image", true);
    }
}

void MainWindow::loadCompressed() {
    QString fileName = QFileDialog::getOpenFileName(this, "Load Compressed Image", "", "Binary Files (*.bin);;All Files (*)");

    if (fileName.isEmpty()) return;

    if (currentImage.loadCompressed(fileName.toStdString())) {
        imageLoaded = true;
        currentImagePath = fileName;
        originalImage = currentImage;
        setStatus("✓ Compressed image loaded and decompressed: " + QFileInfo(fileName).fileName());
        updateImageDisplay();
        showCompressionInfo();
    } else {
        setStatus("✗ Failed to load compressed image", true);
    }
}

void MainWindow::showCompressionInfo() {
    if (!imageLoaded) {
        setStatus("No image loaded", true);
        return;
    }

    int original = currentImage.originalSize();
    int compressed = currentImage.compressedSize();
    float rate = currentImage.compressionRate();

    QString info = QString(
                       "📊 COMPRESSION REPORT\n"
                       "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
                       "📁 Original Size: %1 bytes (%2 KB)\n"
                       "🗜️ Compressed Size: %3 bytes (%4 KB)\n"
                       "📈 Space Saved: %5 bytes (%6 KB)\n"
                       "📉 Compression Rate: %7%%\n"
                       "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
                       ).arg(original)
                       .arg(original / 1024.0, 0, 'f', 2)
                       .arg(compressed)
                       .arg(compressed / 1024.0, 0, 'f', 2)
                       .arg(original - compressed)
                       .arg((original - compressed) / 1024.0, 0, 'f', 2)
                       .arg(rate, 0, 'f', 2);

    QMessageBox::information(this, "Compression Information", info);
    setStatus(QString("✓ Compression rate: %1%").arg(rate, 0, 'f', 2));
}