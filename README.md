# ExifViewer
Simple Qt GUI in C++ for EXIF in jpeg file

## Utilizzo
Scaricare [Qt Creator](https://www.qt.io/download) ed aprire la cartella ExifViewer.
Eseguire il programma (Ctrl + R)


## Esecuzione
* Selezionare una cartella contenente file .jpeg (Shortcut Ctrl + O)
* Verrà mostrata un' immagine alla volta con i relativi EXIF data
* È possibile ruotare l'immagine (Shortcut 'R')
* Si può scorrere le immagini avanti e indietro (Shortcut 'A' indietro, 'D' avanti)
* Se presente nell' exif la posizione GPS si può premere su "Google map position" e si visualizzerà la mappa su browser
* GUI responsive e intuitiva!


![GUI](https://github.com/edoardore/ExifViewer/blob/main/Cattura.PNG)


## Librerie utilizzate
```c++
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QShortcut>
#include <QPainter>
#include <stdio.h>
#include "exif.h"
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
Please make sure to update tests as appropriate.


## License
[Edoardo Re](https://github.com/edoardore), 2021
