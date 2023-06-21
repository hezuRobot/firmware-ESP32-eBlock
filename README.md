Tenemos que preparar el Arduino IDE para poder grabar el firmware.

Dentro de Arduino IDE nos vamos a Archivo>Preferencias.

Damos click al boton de Gestor de tarjetas adicionales y pegamos la siguiente URL:
  https://dl.espressif.com/dl/package_esp32_index.json
  
Le damos a OK y en la ventana previa tambien a OK.

Ahora vamos a Herramientas>Placas>Gestor de Tarjetas se abrirá gestor de tarjetas y tendremos que esperar a que cargue
el contenido que encuentre.

Cuando finalice escribimos esp32 y esperamos a que nos muestre los resultados.

Seleccionamos esp32 by espressive systems y le damos a instalar. Cuando termine cerramos y volvemos a abrir
el Arduino IDE.

A continuacion vamos a Herramientas>Placa y selecionamos DOIT ESP32 DEVKIT V1.

En Herramientas>Puerto seleccionamos el puerto COM donde tengamos conectada nuestra placa.

Solo queda descargar las librerias NonBlockingRTTTL y ESP32Servo cuyas direcciones son:
https://github.com/end2endzone/NonBlockingRTTTL/tree/master
https://github.com/jkb-git/ESP32Servo/tree/master

Para descargar los zip le damos en cada una de esas paginas al boton verde "<> Code" y a continuacion en download zip
Para instalarlo en el Arduino IDE vamos a Programa>Incluir Libreria>Añadir Libreria .zip... y seleccionamos los zip.
Lo haremos de uno en uno.

Reiniciaremos el Arduino IDE cargamos el firmware y solo queda subirlo.

Para ver el resultado es necesario instalar eBlock. Desde este programa podrán comenzar a ejecutar codigo en directo sin 
necesidad de grabar el programa en la placa cada vez que modifiquemos.

Subire en otros repositorios las nuevas extensiones que permiten usar el bluetooth integrado, los puestos tactiles,
servomotores y reproduccion rtttl.

Espero ir añadiendo mas funcionalidades con el tiempo.

Ademas preparare unos videos explicativos de todo el proceso (modificacion del firmware), su uso y las posibles mejoras
mediante actualizacion de firmware y nuevas extensiones.
