# 🖼️ Steganography Tool - Hide & Extract Messages in Images

🔍 A **C-based Steganography project** that **hides and extracts** secret messages from BMP images using **Least Significant Bit (LSB) encoding**.

## 📌 Features
✅ **Encode Message** – Hide text inside a BMP image  
✅ **Decode Message** – Extract hidden text from the image  
✅ **Supports BMP Format** – Works with uncompressed **.bmp** images  
✅ **Lossless Encoding** – Image quality remains almost unchanged  

---

## ⚙️ How It Works

### **1️⃣ Encoding (Hiding Data)**
- Reads an **input text message**  
- Modifies the **Least Significant Bit (LSB)** of image pixels to store message bits  
- Saves the modified image with **hidden data**  

### **2️⃣ Decoding (Extracting Data)**
- Reads the **modified image**
- Validates the password(magic string)
- Extracts message bits from **LSB of pixels**  
- Reconstructs & **prints the hidden message**  

---

## 🛠 Installation & Usage

### **🔹 Requirements**
- **GCC Compiler**
- **Linux or Windows (MinGW for Windows)**

### **🔹 Compilation**
```sh
gcc *.c
ENCODE:
./a.out -e beautiful.bmp secret.txt stego.bmp
DECODE:
./a.out -d stego.bmp
