# 🖼️ Image Viewer & Editor (Qt + C++)

Aplicativo desktop desenvolvido em C++ utilizando o framework Qt para visualização e processamento básico de imagens.

O sistema permite carregar múltiplas imagens, aplicar ajustes de brilho e contraste e realizar interações como zoom, rotação e movimentação (pan) diretamente com o mouse.

---

## 🚀 Funcionalidades

- 📂 Carregamento de múltiplas imagens (PNG, JPG, JPEG, BMP)
- 🎚️ Ajuste de brilho e contraste em tempo real
- 🖱️ Interação com mouse:
  - Pan (movimentação)
  - Zoom
  - Rotação
- 🖼️ Renderização com transformações geométricas
- 💾 Salvamento da imagem editada
- 📋 Lista de imagens carregadas

---

## 🧠 Conceitos Utilizados

- Programação Orientada a Objetos (C++)
- Manipulação de pixels (processamento de imagem)
- Transformações geométricas
- Event handling (eventFilter)
- Renderização com QPainter
- Arquitetura baseada em dados (imagem original vs processada)

---

## 🏗️ Estrutura do Projeto

```
.
├── mainwindow.h
├── mainwindow.cpp
├── mainwindow.ui
├── main.cpp
└── README.md
```

---

## ⚙️ Tecnologias

- C++
- Qt (Qt Widgets)
- STL (vector, algorithm)

---

## 🖥️ Como Executar

### Pré-requisitos

- Qt 6.x instalado
- Compilador C++ (MinGW / MSVC / GCC)

### Passos

```
# Clone o repositório
git clone https://github.com/Gabriel-vsl/cpp-studies.git


# Abra no Qt Creator
# Compile e execute
```

---

## 🎮 Como Usar

1. Clique em **"Carregar Imagem"**
2. Selecione uma ou mais imagens
3. Escolha uma imagem na lista
4. Use os sliders para:
   - Ajustar brilho
   - Ajustar contraste
5. Use o mouse para interagir
6. Selecione o modo:
   - Brilho/Contraste
   - Pan
   - Zoom
   - Rotação
7. Clique em **"Salvar Imagem"** para exportar

---

## 🔄 Pipeline de Processamento

```
Carregar imagem
      ↓
Armazenar (original + processada)
      ↓
Aplicar ajustes (pixels)
      ↓
Aplicar transformações (QTransform)
      ↓
Renderização (QPainter)
      ↓
Exibição
      ↓
Salvar imagem final
```

---

## 👨‍💻 Autor

Gabriel Vieira  
Engenheiro Eletrônico  

---
