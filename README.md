Este firmware nos va a permitir interactuar en directo en eBlock (evolucion de mBlock 3 que es un entorno de programación por bloques).

Tenemos que preparar Arduino IDE para poder grabar el firmware.

Dentro de Arduino IDE nos vamos a Archivo>Preferencias.

Damos click al boton de Gestor de tarjetas adicionales y pegamos la siguiente URL:
  https://dl.espressif.com/dl/package_esp32_index.json
  
Le damos a OK y en la ventana previa tambien a OK.

Ahora vamos a Herramientas>Placas>Gestor de Tarjetas y se abrirá el gestor de tarjetas. Ttendremos que esperar a que cargue
el contenido que encuentre y cuando finalice escribimos esp32 y esperamos a que nos muestre los resultados.

Seleccionamos esp32 by Espressif Systems y le damos a instalar. Cuando termine cerramos y volvemos a abrir
Arduino IDE.

A continuación vamos a Herramientas>Placa y selecionamos DOIT ESP32 DEVKIT V1.

En Herramientas>Puerto seleccionamos el puerto COM donde tengamos conectada nuestra placa.

Solo queda descargar las librerías NonBlockingRTTTL y ESP32Servo cuyas direcciones son:
https://github.com/end2endzone/NonBlockingRTTTL/tree/master
https://github.com/jkb-git/ESP32Servo/tree/master

Para descargar los zip le damos, en cada una de esas páginas, al boton verde "<> Code" y a continuacion a "download zip".
Para instalarlo en Arduino IDE vamos a Programa>Incluir Libreria>Añadir Libreria .zip... y seleccionamos los zip.
Lo haremos de uno en uno.

Reiniciaremos Arduino IDE, cargamos el firmware y solo queda subirlo a la placa.

Para ver el resultado es necesario instalar eBlock en vuestro ordenador y el firmware comentado en el ESP32. Desde eBLock 
podreis comenzar a ejecutar código en directo sin necesidad de grabar el programa en la placa cada vez que modifiquemos.

Subiré en otros repositorios las nuevas extensiones que permiten usar el bluetooth integrado, los puertos tactiles,
los servomotores y la reproduccion rtttl.

Espero ir añadiendo mas funcionalidades con el tiempo.

Además prepararé unos videos explicativos de todo el proceso (modificación del firmware), su uso y las posibles mejoras
mediante actualización de firmware y nuevas extensiones.

Os iré poniendo los enlaces a los videos segun los vaya publicando.

Introducción: https://youtu.be/vhCAriDuhBk

Uso del firmware: https://youtu.be/tp0-Xb8QxxM
