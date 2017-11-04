// Le jeu de Simon
// www.kitco.fr

// Nécessaire pour l'environnement Kitco
#include "kitco.h"


// La durée des sons du jeu
#define DUREE_SON 10

// Les couleur arrière plan et avant plan (inverser BLANC et NOIR pour avoir le jeu en mode inversé)
#define STYLEFG NOIR
#define STYLEBG BLANC
byte score;

#define debug(A)
#define debug(A) Serial.println(A) 


//Fréquences des sons de chaque couleur
int son[] = {261, 330, 392, 523};
//            Do,  Mi, Sol,  Do

//Tableau de séquence à jouer
int sequence[50];
//Personne ne dépasse jamais le niveau 50...

//---------------------------------------------------------
//      jouerSequence
//
//Joue toute la séquence
void jouerSequence(int niveau)
{
  for (int i = 0; i <= niveau; i++)
  {
    jouerNote(sequence[i]);
  }
}
//---------------------------------------------------------
//      nouvelleNote
//
//Nouvelle note aléatoire
void nouvelleNote(int niveau)
{
  sequence[niveau] = (int)random(1, 5);
}


// Une partie commence
void nouvellePartie() {
  score = 0;
  afficheJeu();
}

// -------------------------------------
//    Setup
// La partie Setup concerne ce qui va être exécuté au démarrage de Kitco
void setup() {

   Serial.begin(9600);
   
  // Cette commande est nécessaire pour intialiser Kitco à son démarrage 0 en mode dev et 1 en mode production
  initialiserKitco(1);
  lcdBegin();
  setContrast(60);

  debug("SETUP");

  afficheJeu();
  creerRectangle(20, 5 , 64, 23, true, STYLEBG);
  creerRectangle(21, 6 , 63, 22, true, STYLEFG);
  ecrireEcran("SIMON", 27, 12, STYLEBG);

  creerRectangle(2, HAUTEUR_ECRAN/3 * 2 + 3 , 82, HAUTEUR_ECRAN - 1, true, STYLEFG);
  creerRectangle(3, HAUTEUR_ECRAN/3 * 2 + 4 , 81, HAUTEUR_ECRAN - 2, true, STYLEBG);
  ecrireEcran("chante42@2017", 3, HAUTEUR_ECRAN/3*2 + 6, STYLEFG);
  updateDisplay();
  
}

// -------------------------------------
//           afficheScore()
//
void afficheScore() {
  ecrireLettre('0' + score % 10, LARGEUR_ECRAN / 3 + 3 + 9, HAUTEUR_ECRAN / 3 + 5 , STYLEFG );
  if (score >= 10) ecrireLettre('0' + (score / 10) % 10, LARGEUR_ECRAN / 3 + 3 + 6, HAUTEUR_ECRAN / 3 + 5 , STYLEFG );
  
  updateDisplay();

}

#define afficheNote1(c) afficheNoteXY(0                     , HAUTEUR_ECRAN / 3     ,c);
#define afficheNote2(c) afficheNoteXY(LARGEUR_ECRAN / 3     , 0                     ,c);
#define afficheNote3(c) afficheNoteXY(LARGEUR_ECRAN / 3 * 2 , HAUTEUR_ECRAN / 3     ,c);
#define afficheNote4(c) afficheNoteXY(LARGEUR_ECRAN / 3     , HAUTEUR_ECRAN / 3 * 2 ,c);

// -------------------------------------
//          afficheNoteXY
//
void afficheNoteXY(byte x, byte y, byte couleur) {

  creerRectangle(x ,  y , x + LARGEUR_ECRAN / 3 , y + HAUTEUR_ECRAN / 3 , false, STYLEFG);
  // centre la note
  x += 9;
  y += 2;


  creerRectangle(x ,  y + 4 , x + 4, y + 8 , true, couleur);
  creerRectangle(x + 8,  y + 8 , x + 12, y + 12 , true, couleur);

  // barre vericale 1
  setPixel(x + 2, y + 0, couleur);
  setPixel(x + 2, y + 1, couleur);
  setPixel(x + 2, y + 2, couleur);
  setPixel(x + 2, y + 3, couleur);

  // barre vericale 2
  setPixel(x + 10, y + 4, couleur);
  setPixel(x + 10, y + 5, couleur);
  setPixel(x + 10, y + 6, couleur);
  setPixel(x + 10, y + 7, couleur);

  setPixel(x + 3, y + 1, couleur);
  setPixel(x + 4, y + 2, couleur);
  setPixel(x + 5, y + 3, couleur);
  setPixel(x + 6, y + 3, couleur);
  setPixel(x + 7, y + 4, couleur);
  setPixel(x + 8, y + 4, couleur);
  setPixel(x + 9, y + 4, couleur);

}

// -------------------------------------
//      afficheJeu
//
// Gestion des graphismes
void afficheJeu() {

  clearDisplay(STYLEBG);

  // on dessine les 4 rectangle
  creerRectangle(LARGEUR_ECRAN / 3, 0, LARGEUR_ECRAN / 3 * 2, HAUTEUR_ECRAN / 3 , true, STYLEFG);

  creerRectangle(LARGEUR_ECRAN / 3,  HAUTEUR_ECRAN / 3 * 2 , LARGEUR_ECRAN / 3 * 2, HAUTEUR_ECRAN , true, STYLEFG);

  creerRectangle(0,  HAUTEUR_ECRAN / 3 , LARGEUR_ECRAN / 3, HAUTEUR_ECRAN / 3 * 2 , true, STYLEFG);
  creerRectangle(LARGEUR_ECRAN / 3 * 2,  HAUTEUR_ECRAN / 3 , LARGEUR_ECRAN, HAUTEUR_ECRAN / 3 * 2 , true, STYLEFG);

  // On affiche le score
  setChar(48 + 0, LARGEUR_ECRAN / 3 + 5 , HAUTEUR_ECRAN + 3, STYLEFG);

  afficheNoteXY(0                     , 0                         , STYLEFG);
  afficheNoteXY(LARGEUR_ECRAN / 3 * 2 , 0                         , STYLEFG);
  afficheNoteXY(0                     , HAUTEUR_ECRAN / 3 * 2     , STYLEFG);
  afficheNoteXY(LARGEUR_ECRAN / 3 * 2 , HAUTEUR_ECRAN / 3 * 2     , STYLEFG);
  
  afficheScore();
}

// -------------------------------------
//         lectureBouton
//
// Gestion des appuis touches
int lectureBouton() {

  int boutonPresse = 0;
  debug("lectureBouton Debut");

  while (!boutonPresse)
  {
    // touche Haut 
    if (toucheHaut() ) {
      boutonPresse = 2;
    }

    // touche bas 
    if (toucheBas() ) {
      boutonPresse = 4;
    }

    // touche Gauche
    if (toucheGauche() ) {
      boutonPresse = 1;
    }

    // touche Droite
    if (toucheDroite() ) {
      boutonPresse = 3;
    }

    // touche bas A 
    if (toucheA() ) {
      boutonPresse = 254;
    }

    // touche bas B
    if (toucheB() ) {
      boutonPresse = 254;
    }
  } // FIN WHILE

  return boutonPresse;
}

// -------------------------------------
//    logiqueJeu
//
// gestion de la logique du jeu
void logiqueJeu() {

  randomSeed(analogRead(A0));

  //Attente de pression sur une touche.
  lectureBouton();

  
  //Témoin de lancement du jeu.
  afficheJeu();
  melodieLancement();
  delay(2000);

  
  //Début du jeu
  boolean gameOver = false;
  int presse;
  int niveau = 0;
  nouvellePartie();
  while (!gameOver)
  {
    debug("while !gameover");
    nouvelleNote(niveau); //Rajoute une note à la séquence.
    jouerSequence(niveau); //Joue la séquence

    for (int i = 0; i <= niveau; i++) //Attend que le joueur tape chaque note de la séquence
    {
      debug("joue Niveau");
      presse = lectureBouton(); //Lecture du bouton appuyé par le joueur
      jouerNote(presse); //Joue la note du joueur
      if (presse != sequence[i]) //Si la note est bonne, on continue, sinon GameOver
      {
        gameOver = true;
        finPartie();
      }
      if (gameOver) {
        break;
      }
    }
    score = niveau;
    niveau++;
    afficheScore();
    delay(1000);
  }

}

//---------------------------------------------------------
//           jouerNote
//
void jouerNote(int numero)
{
  switch (numero)
  {
    case 1:
      afficheNote1(STYLEBG);
      break;
    case 2:
      afficheNote2(STYLEBG);
      break;
    case 3:
      afficheNote3(STYLEBG);
      break;
    case 4:
      afficheNote4(STYLEBG);
      break;
  }
  updateDisplay();
  frequenceBuzzer(son[numero - 1], 300); //Le tableau commence à 0 (non à 1)
  delay(300);

  afficheNote1(STYLEFG);
  afficheNote2(STYLEFG);
  afficheNote3(STYLEFG);
  afficheNote4(STYLEFG);
  updateDisplay();
  delay(200);
}

//---------------------------------------------------------
//      melodieLancement
//
//Témoin lancement (petite séquence lumineuse)
void melodieLancement()
{
  afficheNote1(STYLEBG);
  updateDisplay();
  frequenceBuzzer(son[0], 300);
  delay(50);

  afficheNote1(STYLEFG);
  afficheNote2(STYLEBG);
  updateDisplay();
  frequenceBuzzer(son[1], 300);
  delay(50);

  afficheNote2(STYLEFG);
  afficheNote3(STYLEBG);
  updateDisplay();
  frequenceBuzzer(son[2], 300);
  delay(50);


  afficheNote3(STYLEFG);
  afficheNote4(STYLEBG);
  updateDisplay();
  frequenceBuzzer(son[3], 300);
  delay(300);


  afficheNote4(STYLEFG);
  updateDisplay();
}

//---------------------------------------------------------
//            finPartie
//Témoin Game Over
void finPartie()
{
  int i;
  for (i = 3 ; i >= 0; i--) {
    afficheNote1(STYLEBG);
    afficheNote2(STYLEBG);
    afficheNote3(STYLEBG);
    afficheNote4(STYLEBG);
    updateDisplay();
    frequenceBuzzer(son[i]*2, 300);
    delay(50);
    
    afficheNote1(STYLEFG);
    afficheNote2(STYLEFG);
    afficheNote3(STYLEFG);
    afficheNote4(STYLEFG);
    updateDisplay();
    delay(50);
  } // fin FOR

  creerRectangle(20, 5 , 64, 23, true, STYLEBG);
  creerRectangle(21, 6 , 63, 22, true, STYLEFG);
  ecrireEcran("PERDU", 27, 12, STYLEBG);
  updateDisplay();
}




// Loop est la boucle principale, va se lancer en boucle après Setup
void loop() {
  logiqueJeu();
}

