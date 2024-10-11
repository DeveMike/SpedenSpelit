# Speden Spelit reaktiopeli – Arduino Edition

Tämä projekti on osa Oulun ammattikorkeakoulun ”Tietotekniikan sovellusprojekti” -kurssia.  Speden Spelit on Arduino-pohjainen reaktio- ja nopeuspeli, joka on saanut inspiraationsa suomalaisesta Speden Spelit-ohjelman peleistä. Pelissä pelaaja testaa reaktioaikaansa ja nopeuttaan yrittäen painaa syttyvää lediä vastaavaa nappia tarpeeksi nopeasti saadakseen pisteen. Peli alkaa, kun pelaaja pitää oikeanpuoleista alanappia 2 sekuntia painettuna. Tämän jälkeen kuuluu aloitusääni ja peli alkaa. LEDien vilkkumistahti on alussa 1 Hz ja tahtia nopeutetaan 10 % kymmenen oikean painalluksen jälkeen. Peli loppuu, jos pelaaja painaa väärin tai jättää painamatta. Peli tallentaa parhaan pistemäärän, ja paras tulos näkyy 7-segmenttinäytöllä. Jos laitetta ei käytetä minuuttiin, se menee virransäästötilaan. Laite herää, kun pelaaja painaa oikeanpuoleista alanappia kerran.

![promo2](https://github.com/user-attachments/assets/f8ad9e48-5bbe-4b00-b72d-6d8116a723de)

# Sisältö

# Tekniset tiedot
•	Kytkentäkaavio: Projekti sisältää kytkentäkaavion, joka näyttää, miten pelin elektroniikkakomponentit, kuten painikkeet, LED-valot, summerit, 7-segment taulut sekä siirtorekisterit on kytketty Arduinoon. 

•	Vuokaavio: Selittää pelin toimintalogiikan ja erilaisten toimintojen vuorovaikutuksen, mikä auttaa hahmottamaan pelin kulkua.


•	Kooditiedostot: Ohjelmakoodi on jaettu modulaarisiin osiin, kuten painikkeiden, näyttöjen, pisteiden laskennan ja ääniefektien käsittelyyn.

•	Videoesitys: Käytännön demonstraatio, joka näyttää valmiin laitteen toiminnassa.

# Asennus
•	Asennusta varten tulet tarvitsemaan seuraavat komponentit: Arduino Uno – tai vastaava, 4kpl BROADCOM HLMP-4740 LED – tai vastaava, 4kpl PANASONIC ESE20C321 Painiketta, 2kpl KINGBRIGHT SC52-11EWA 7-segment näyttöä, 2kpl TEXAS INSTRUMENTS SN74HC595N Siirtorekisteriä, Crowtail KAIUTIN, tarvittava määrä hyppylankoja, vastuksia sekä riittävän ison kytkentäalustan. Halutessasi voit myös juottaa komponentit piirilevylle ja koteloida laitteen.
![Mediasisältö](https://github.com/user-attachments/assets/0fc59c87-1206-473f-9d1a-f863519c6c72)

•	Kytkentä tulee suorittaa kytkentäkaavion mukaisesti, jotta peli tulee toimimaan ohjelmistokoodin mukaisesti. Erityishuomiota tarvitaan painikkeiden sekä LED valojen kanssa, jotta nämä ovat kytketty oikeisiin portteihin Arduino:ssa

•	Pelin toteutusta varten tarvitset tietokoneellesi ARDUINO IDE ohjelmiston. Ohjelmiston kautta voit ladata kaikki tiedostot suoraa Arduino:lle.

•	Ohjelmakoodin lataaminen SpedenSpelit_v2_6.ino -tiedostosta ja tarvittavien .cpp ja .h tiedostojen lisääminen.
