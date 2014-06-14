#include "dossier.h"
#include "dataBaseA.h"

#include <QDialog>
#include <QtGui>
#include <QtXml>
#include <QWidget>
#include <QMessageBox>
#include <QMainWindow>

using namespace question3;
using namespace UV_credits_types;

//les Categorie sont maitenant des CreditType et sont implémentés dynamiquement
/*
Categorie question3::StringToCategorie(const QString& str){
    if (str=="CS") return CS;
    else
    if (str=="TM") return TM;
    else
    if (str=="SP") return SP;
    else
    if (str=="TSH") return TSH;
    else {
        throw UTProfilerException(QString("erreur, StringToCategorie, categorie ")+str+" inexistante");
    }
}

QString question3::CategorieToString(Categorie c){
    switch(c){
    case CS: return "CS";
    case TM: return "TM";
    case SP: return "SP";
    case TSH: return "TSH";
    default: throw UTProfilerException("erreur, categorie non traitee",__FILE__,__LINE__);
    }
}
*/
Saison question3::StringToSaison(const QString str){
    if (str=="A") return Automne;
    else
    if (str=="P") return Printemps;
    else {
        throw UTProfilerException(QString("erreur, StringToSaison, saison ")+str+" inexistante");
    }
}

Note question3::StringToNote (const QString str){
    if (str=="A") return A;
    else
    if (str=="B") return B;
    else
    if (str=="C") return C;
    else
    if (str=="D") return D;
    else
    if (str=="E") return E;
    else
    if (str=="FX") return FX;
    else
    if (str=="F") return F;
    else
    if (str=="EC") return EC;
    else {
        throw UTProfilerException(QString("erreur, StringToNote, note ")+str+" inexistante");
    }

}

Dossier* Dossier::instance=0;//initialisation de la variable static


Dossier::Dossier():login_etudiant(""), modification(false){
    //création d'un nouveau dossier
}

Dossier& Dossier::getInstance(){
    if(!instance)
        instance=new Dossier;
    return * instance;
}

void Dossier::libererInstance(){
    if(instance){
        delete instance;
        instance=0;
    }
}

//!!!!!!!! A FAIRE!!!!!!!!
//c'est bon, je l'ai implémenté dans le .h! (nico)
//Dossier::~Dossier(){
//    if(modification){
//        //enregistrement du dossier



//    }
//}


XmlStreamReader::XmlStreamReader(Dossier* doss,Cursus* rootCursus):rootCursus(rootCursus){

    dossierAremplir = doss;

}



bool XmlStreamReader::readFile(const QString &fileName){
    QFile xml_doc(fileName);// On choisit le fichier contenant les informations XML.
    if(!xml_doc.open(QIODevice::ReadOnly | QIODevice::Text))// Si l'on n'arrive pas à ouvrir le fichier XML.
    {
        QMessageBox::warning(0,"Erreur à l'ouverture du document XML","Le document XML n'a pas pu être ouvert.");
        return false;
    }
    reader.setDevice(&xml_doc);
    QMessageBox::information(0,"   ", "entrée readFile");



    // We'll parse the XML until we reach end of it.
    while(!reader.atEnd() && !reader.hasError()) {
        // Read next element.
        QXmlStreamReader::TokenType token = reader.readNext();

        // If token is just StartDocument, we'll go to next.
        if(token == QXmlStreamReader::StartDocument) continue;
        // If token is StartElement, we'll see if we can read it.
        if(token == QXmlStreamReader::StartElement) {
            // If it's named dossier, we'll dig the information from there.
            if(reader.name() == "dossier") {
                readDossier();
            }
            else
                reader.readNext();
        }
    }
    // Error handling.
    if(reader.hasError()) {
        throw UTProfilerException("Erreur lecteur fichier dossier, parser xml");
    }
    xml_doc.close(); // Dans tous les cas, on doit fermer le document XML : on n'en a plus besoin, tout est compris dans l'objet
    return true;
}

void XmlStreamReader::readDossier(){
    reader.readNextStartElement();

    unsigned int i=0;
    while(!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "dossier")) {

        if(reader.tokenType() == QXmlStreamReader::StartElement) {
            // We've found login.
            if(reader.name() == "login") {
                reader.readNext();
                QString l=reader.text().toString();
                dossierAremplir->setLogin_etudiant(l);
            }

            if(reader.name() == "cursus") {
                /*
                reader.readNext();
                Cursus_Etudiant* ce= new Cursus_Etudiant(rootCursus);
                readRecursiveCursus(ce);
                dossierAremplir->setCursus(ce);


                */
            }
            // We've found inscription.
            if(reader.name() == "inscription") {
                reader.readNext();
                Inscription in;
                dossierAremplir->setInscr(readInscription(in));
            }

            // We've found equivalence.
            if(reader.name() == "equivalence") {
                reader.readNext();
                Equivalence equi;
                dossierAremplir->setEqui(readEquivalence(equi));
            }


            // We've found solution.
            if(reader.name() == "solution") {
                reader.readNext();
                QList<Prevision> lP=readSolution();
                dossierAremplir->setMapSolutions(i,lP);
                i++;


            }

        }
        reader.readNext();
    }

}




void readRecursiveCursus(Cursus_Etudiant* parent){

    //QTreeWidgetItem *item = new QTreeWidgetItem(parent);
      //  item->setText(0, reader.attributes().value("term").toString());

    //fonction pour recopier le cursus (voir feuolle)
//    for (int j=0; j<parent->getCursusReference()->getSOUSCursusList().length(); j++){
//        if(!strcmp(parent->getCursusReference()->getSOUSCursusList().at(j)->getName(),cursus)){
//            QMessageBox::information(0," ","");
//            Cursus* selected=parent->getCursusReference()->getSOUSCursusList().at(j);
//            Cursus_Etudiant* sous_cursus=new Cursus_Etudiant(selected);
//            parent->addSousCursus(sous_cursus);
//        }
//     }
//        reader.readNext();
//        while (!reader.atEnd()) {
//            if (reader.isEndElement()) {
//                reader.readNext();
//                break;
//            }




//            if (reader.isStartElement()) {
//                if (reader.name() == "cursus") {
//                    readRecursiveCursus(sous_cursus);

//                } else {
//                    skipUnknownElement();
//                }
//            } else {
//                reader.readNext();
//            }
//        }



}

Inscription& XmlStreamReader::readInscription(Inscription& inscri){

    while(!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "inscription")){

        if(reader.tokenType() == QXmlStreamReader::StartElement) {
            if(reader.name()=="semestreI"){
                reader.readNext();
                Semestre sem;
                inscri.setSemestre(readSemestre(sem));



            }
            if(reader.name()=="resultat"){
                reader.readNext();
                inscri.setResultat(StringToNote(reader.text().toString()));
            }
            if(reader.name()=="uvI"){
                reader.readNext();
                inscri.setUv(readUv());
            }

         }
         reader.readNext();
    }

   return inscri;
}

Semestre& XmlStreamReader::readSemestre(Semestre& sem){



    while(!(reader.tokenType() == QXmlStreamReader::EndElement && (reader.name() == "semestreI" ||reader.name() == "semestreP"))) {

        if(reader.tokenType() == QXmlStreamReader::StartElement) {
            if(reader.name()=="saison"){
                reader.readNext();
                sem.setSaison(question3::StringToSaison(reader.text().toString()));
            }
            if(reader.name()=="annee"){
                reader.readNext();
                sem.setAnnee(reader.text().toString().toUInt());
            }

        }
        reader.readNext();
    }

    return sem;
}


UV* XmlStreamReader::readUv(){
    /*A decommenter quand UVManager sera implémentée
    QString code;
    //lire le code dans fichier xml
    code=reader.text().toString();
    //le chercher dans UVManager
    while(uvs[iterator]->code!=code){
        iterator++;
    }
    //retourner le pointeur
    return uvs[iterator];
*/
}

Equivalence& XmlStreamReader::readEquivalence(Equivalence& equi){

    while(!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "equivalence")){
        if(reader.tokenType() == QXmlStreamReader::StartElement) {
            if(reader.name()=="portee"){
                reader.readNext();
                equi.setPortee(reader.text().toString());
            }
            if(reader.name()=="credits"){
                reader.readNext();
                Credits cred;
                readCredits(cred);
            }

        }
        reader.readNext();
    }
    return equi;
}



Credits& XmlStreamReader::readCredits(Credits &cred){

    while(!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "credits")) {
        if(reader.tokenType() == QXmlStreamReader::StartElement) {

            if(reader.name()=="nb"){
                reader.readNext();
                cred.setNbCredits(reader.text().toString().toUInt());
                }
            if(reader.name()=="typeCredit"){
                reader.readNext();
                /*QString credTy=reader.text().toString()
                if(credTy nexiste pas encore)//chercher dans la QList de CreditType Manager
                    CreditType* CT=new CreditType(credTy);
                else
                    CreditType* CT=&de ce type qui existe déjà//chercher dans CreditType Manager
                cred.setType(CT);
                */
            }

        }
        reader.readNext();
    }
    return cred;
}



QList<Prevision> XmlStreamReader::readSolution(){

   QList<Prevision> solution;
    while(!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "solution")){
        if(reader.tokenType() == QXmlStreamReader::StartElement) {
            if(reader.name()=="prevision"){
                reader.readNext();
                Prevision prev;
                solution.push_back(readPrevision(prev));
            }

        }
        reader.readNext();
    }
    return solution;
}


Prevision& XmlStreamReader::readPrevision(Prevision& prev){

    while(!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "prevision")){
        if(reader.tokenType() == QXmlStreamReader::StartElement) {
            if(reader.name()=="semestreP"){
                reader.readNext();
                Semestre sem;
                prev.setSemestre(readSemestre(sem));
            }
            if(reader.name()=="uvP"){
                reader.readNext();
                prev.setUv(readUv());
            }

        }
        reader.readNext();
    }
    return prev;
}

void XmlStreamReader::skipUnknownElement(){

}


using namespace UV_credits_types;
//using namespace FOOO; //TODO nico: supprimer cette ligne à la fin

float XUVParmi::completion_percentage (QList<Inscription> *ti) const{

    unsigned int cpt=0;
    string a,b;

    // copare dans chacune des deux listes le nombres d'UVs en commun et place le résultat dans cpt
    if(ti->length() > 0){
        for(    int i=0 ; i<   ti->length() ; i++){
            for(int j=0 ; j<UVlist.length() ; j++){
                a = ti->at(i).getUV().getCode().toStdString();
                b = UVlist.at(j)->getCode().toStdString();
                //std::cout+a+" et "+b+" :"+strcmp(a.c_str(),b.c_str()) +"\n";
                if( strcmp(a.c_str(),b.c_str())==0 ){  // comparaison des deux codes d'UV
                    cpt++;
                    j = UVlist.length();
                }
                // TODO NICO : ettention ici on ne gere pas les doublons d'UV!! Ni les listes nulles
            }
            //std::cout+"_________________"+cpt+"____________________________\n";
        }
    }

    if(cpt>nb) return 1; // si l'étudiant à plus d'UV que nécessaire, le pourcentage de completion est de 100.
    return (float)((float)cpt/(float)nb);
    // TODO NICO :gerer le fx
}






void XUVParmi::copyIntoQtRuleView(QStandardItem * item)const{
    testNullPtr(false,false);
    item->setText(string(this->toString()+" ("+this->getName()+")").c_str());
};

void XCreditsParmi::copyIntoQtRuleView(QStandardItem * item)const{
    //QMessageBox::warning(0," ","hehe");
    testNullPtr(false,false);
    //QMessageBox::warning(0," ","hehe2");
    //QMessageBox::warning(0," ",this->getName());
    //QMessageBox::warning(0," ",this->toString().c_str());
    item->setText(string(this->toString()+" ("+this->getName()+")").c_str());
    //QMessageBox::warning(0," ","hehe3");
};

float XCreditsParmi::completion_percentage (QList<Inscription> *ti) const{

    unsigned int cpt=0;
    QList<Credits*>* allCredits=0;

    DATABASE::UnpersistentDataBaseA db1 = DATABASE::UnpersistentDataBaseA();//TODO NICO : supprimer cette ligne a la fin!!!

    // parcours toutes les UVs de l'étudiant, et va chercher (et stocke dans "allCredits") pour chacune d'elle le nombre de crédits qu'elle rapport dans chaque type (ex: l'UV LO21 rapporte 6 crédits de type TM)
    if(ti->length() > 0){
        for( int i=0 ; i <ti->length() ; i++){ // parcours de toutes les UVs de l'étudiant
            for (int j = 0 ; j<porteeList.length() ; j++){ //^parcours des portees definies pour la présente règle de validation
              *allCredits << *db1.returnCreditsOfAnUV( ti->at(i).getUV().getCode().toStdString() , porteeList.at(j)->getLibelle().toStdString() ) ; // TODO NICO: utiliser le DP visitor
            }
        }
    }

    if(allCredits->length()>0) // On dépouille tous les crédits accumulés lors de l'analyse pour en faire la somme.
        for(int k=0; k<allCredits->length(); k++)
            cpt = cpt + allCredits->at(k)->getNbCredits();

    if(cpt>nb) return 1; // si l'étudiant à plus de crédits que nécessaire, le pourcentage de completion est de 100.
    return (float)((float)cpt/(float)nb);
}





float FonctionOU::completion_percentage (QList<Inscription> *ti) const{
    // on renvoie le maximum de tous les pourcentages des règles contenues dans la liste de règles
    float max = 0;
    float a;

    if(VRlist.length() > 0){
        for(int i=0; i<VRlist.length() ; i++){
            if(max < (a=VRlist.at(i)->completion_percentage(ti)) ){
                max = a;
            }
        }
    }
    return(max);
}

void FonctionOU::copyIntoQtRuleView(QStandardItem * item)const{
    testNullPtr(false,false);
    ValidationRule* vr;
    QList<QStandardItem*> subItem;
    item->setText((string("Fonction OU (")+string(this->getName())+string(")")).c_str());
    if(VRlist.length() >0){
        for(int j=0; j<VRlist.length() ; j++){
            vr = VRlist.at(j);
            subItem << new QStandardItem();
            vr->copyIntoQtRuleView(subItem.at(j));
            item -> appendRow(subItem.at(j));
        }
    }
};




float FonctionET::completion_percentage (QList<Inscription> *ti) const{

    float produit=0;
    int cpt=0;

    //parcours de toutes les regles de validation.
    //Le pourcentage total est le produit des pourcentages respectifs de toutes ces règles.
    if( VRlist.length() >= 1){
        for( int i=0 ; i <VRlist.length() ; i++){ // parcours de toutes les UVs de l'étudiant
            cpt++;
            produit +=  VRlist.at(i)->completion_percentage(ti);
        }
    }

    if(cpt){
        return (float)((float)produit/(float)cpt); // on fait ici la moyenne de tous les pourcentages
    }else{
        return 1;
    }

}

void FonctionET::copyIntoQtRuleView(QStandardItem * item)const{
    testNullPtr(false,false);
    ValidationRule* vr;
    QList<QStandardItem*> subItem;
    item->setText((string("Fonction ET (")+string(this->getName())+string(")")).c_str());
    if(VRlist.length() >0){
        for(int j=0; j<VRlist.length() ; j++){
            vr = VRlist.at(j);
            subItem << new QStandardItem();
            vr->copyIntoQtRuleView(subItem.at(j));
            item -> appendRow(subItem.at(j));
        }
    }
};




string XUVParmi::toString () const {

    string str = string("");

    //std::ostringstream nbSTR= std::ostringstream();
    //nbSTR << nb;

   if(UVlist.length()>0){
        int i;
          str = string("Obtenir ") + string(" UV parmi : ") ; // TODO NICO: corriger ici
        //str = "Avoir obtenu " + nbSTR.str() +" UV parmi : " ;
       for(i=0; i<(UVlist.length()-1) ; i++)
            str =str + string(UVlist.at(i)->getCode().toStdString()) + "," ;
        i = UVlist.length()-1;
        str =str + UVlist.at(i)->getCode().toStdString() + "." ;

    }else{
        str = "!!Règle incomplète";
    }

    return str;
}

string XCreditsParmi::toString () const {

    string str = string("");

//	std::ostringstream oss;
//    nbSTR << nb;

    if(typeList.length()>0){
 //QMessageBox::warning(0," ",this->getName());
        int i;
        str = string("Obtenir ") + string(" créd. (") ;
 //QMessageBox::warning(0," ","p21");
        for(i=0; i<(typeList.length()-1) ; i++){
            //QMessageBox::warning(0," ","p22boucle");
            //QMessageBox::warning(0," ",typeList.at(i)->getLibelle());
            str = str + typeList.at(i)->getLibelle().toStdString() + " ou " ;    
        }
 //QMessageBox::warning(0," ","p23");
        i = typeList.length()-1;
 //QMessageBox::warning(0," ","p24");
        str = str + typeList.at(i)->getLibelle().toStdString() + ") parmi UV ayant une portée dans: (" ;
 //QMessageBox::warning(0," ","p3");
        for( i=0 ; i<(porteeList.length()-1) ; i++)
                str = str + porteeList.at(i)->getLibelle().toStdString() + ", " ;
            i = porteeList.length()-1;
            str = str + porteeList.at(i)->getLibelle().toStdString() + ")." ;
    }else{
        str = "!!Règle incomplète";
    }

    return str;

}

string FonctionOU::toString () const {

    string str = string("");

    if(VRlist.length()>0){
        int i;
        str = str + " [ ";
        for(i=0; i<VRlist.length()-1 ; i++)
            str = str + VRlist.at(i)->toString() + " ]\nOU [ ";
        i = VRlist.length()-1 ;
        str = str + VRlist.at(i)->toString() + " ]";

    }else{
        str = "";
    }

    return str;
}

string FonctionET::toString () const {

    string str = string("");

    if(VRlist.length()>0){
        int i;
        str = str + " [ ";
        for(i=0; i<VRlist.length()-1 ; i++)
            str = str + VRlist.at(i)->toString() + " ] \nET [ ";
        i = VRlist.length()-1 ;
        str = str + VRlist.at(i)->toString() + " ]";

    }else{
        str = "";
    }

    return str;
}


float Profil::completion_percentage (QList<Inscription> *ti) const{

    float produit=0;
    int cpt=0;

    //parcours de toutes les regles de validation.
    //Le pourcentage total est le produit des pourcentages respectifs de toutes ces règles.
    if(VRlist.length() > 0){
        for( int i=0 ; i <VRlist.length() ; i++){ // parcours de toutes les UVs de l'étudiant
            produit = produit + VRlist.at(i)->completion_percentage(ti);
            cpt ++;
        }
    }

    if(cpt){
        return (float)((float)produit/(float)cpt);  // on fait la moyenne de tous les pourcentages
    }else{
        return 1;
    }

}

string Profil::toString(int k) const{

    string indentation=string("");
    for(int l=0;l<k;l++)
        indentation = indentation + "      ";
    string str = indentation;

    if(VRlist.length()>0){
        int i;
        for(i=0; i<VRlist.length() ; i++)
            str = str +"\n"+indentation+"règle de validation:" + VRlist.at(i)->toString();
    }else{
        str = "";
    }

    return str;
}

void Profil::copyIntoQtRuleView(QStandardItemModel *modeleRegl)const{
    testNullPtr(false,false);
    ValidationRule* vr;
    QList<QStandardItem*> subItem;
    if(VRlist.length() >0){
        //QMessageBox::warning(0," ","\non passe ici");
        for(int j=0; j<VRlist.length() ; j++){
           // QMessageBox::warning(0," ","\net ici aussi1");
            vr = VRlist.at(j);
            //QMessageBox::warning(0," ",vr->getName());
            //QMessageBox::warning(0," ","\net ici aussi2");
            //subItem << new question3::QStandardItem_Regle(vr->getName(),VRlist.at(j));
            //QMessageBox::warning(0," ","\net ici aussi3");
            subItem << new QStandardItem();
            //QMessageBox::warning(0," ","\net ici aussi4");
            //if(vr==0)QMessageBox::warning(0," ","\nTrouvé!!");
            vr->copyIntoQtRuleView(subItem.at(j));
            //QMessageBox::warning(0," ","\net ici aussi5");
            modeleRegl->appendRow(subItem.at(j));
        }
    }
}


#include <ctime>//TODO NICO

float Cursus::completion_percentage (QList<Inscription> *ti) const{

    float somme;
    int cpt=0;

    //parcours de toutes les regles de validation.
    //Le pourcentage total est le produit des pourcentages respectifs de toutes ces règles.
    if(PROlist.length() > 0){
        for( int i=0 ; i <PROlist.length() ; i++){ // parcours de toutes les UVs de l'étudiant
            somme = somme + PROlist.at(i)->completion_percentage(ti);
            cpt++;

        }
    }
    if(SOUS_cursus.length() > 0){
        for( int j=0 ; j <SOUS_cursus.length() ; j++){ // parcours de toutes les UVs de l'étudiant
            somme = somme + SOUS_cursus.at(j)->completion_percentage(ti);
            cpt++;

        }
    }

    if(cpt){
        return (float)((float)somme/(float)cpt); ; // On fait la moyenne du pourcentage de complétion des sous-cursus ainsi que la moyenne des pourcentages de complétion du profil du présent cursus.
    }else{
        return 1;
    }
}

string Cursus::toString(int k) const{

    string indentation=string("");
    for(int l=0;l<k;l++)
        indentation = indentation + "      ";
    string str = indentation;

    if(PROlist.length()>0){
        int i;
        for(i=0; i<PROlist.length() ; i++)
            str = str + "\n"+indentation+"Profil \""+PROlist.at(i)->getName()+"\": " + PROlist.at(i)->toString(k+1);

        if(SOUS_cursus.length() >0){
            str = str + "\n\n"+indentation+"Mais il faut également ";
        }
    }
    if(SOUS_cursus.length() >0){
        int j;
        str = str + "Valider les Sous-Cursus suivants : ";
        for(j=0; j<SOUS_cursus.length()-1 ; j++)
            str = str + SOUS_cursus.at(j)->getName() + ", ";
        j = SOUS_cursus.length()-1 ;
        str = str + SOUS_cursus.at(j)->getName() + ".";

        str = str + "\n"+indentation+"Voici donc les conditions de validation de ces sous-cursus:";
        for(j=0; j<SOUS_cursus.length() ; j++)
            str = str +"\n"+indentation+"sous-cursus \""+SOUS_cursus.at(j)->getName()+"\": "+ SOUS_cursus.at(j)->toString(k+1) + ",\n";

    }

    return str;
}


void Cursus::copyIntoQtProfilView(QStandardItemModel *modeleProfil,int begin)const{
    //std::cout<<"\n\n--AA-- nb de sous cursus :"<<SOUS_cursus.length();
    testNullPtr(false,false);
    for(int i=begin-1; i<PROlist.length() ; i++){
        //QMessageBox::warning(0," ","prolist.length passage\n");
        modeleProfil->appendRow(new QStandardItem_Profil(PROlist.at(i)->getName(),PROlist.at(i)));
    }

}


void Cursus::copyIntoQtCursusView(QStandardItem *itemCursus)const{
    testNullPtr(false,false);
    if(SOUS_cursus.length() >0){
        for(int j=0; j<SOUS_cursus.length() ; j++){
            Cursus* currentCurs = SOUS_cursus.at(j);
            QStandardItem *subItem = new QStandardItem_Cursus(currentCurs->getName(),SOUS_cursus.at(j));
            currentCurs->copyIntoQtCursusView(subItem);
            itemCursus -> appendRow(subItem);
        }
    }

}

/*void copyIntoQtCursusView(QStandardItem *itemCursus)const{

    if(SOUS_cursus.length() >0){
        for(int j=0; j<SOUS_cursus.length() ; j++){
            Cursus* currentCurs = SOUS_cursus.at(j);
            //std::cout<<"\n\n--AA-- on traite ici le sc "<< currentCurs->getName();
            QStandardItem *subItem = new QStandardItem(currentCurs->getName());
            //std::cout<<"\n\n--AA-- Entree dans la fn rec";
            currentCurs->copyIntoQtCursusView(subItem);
            //std::cout<<"\n\n--AA-- Sortie de la fn rec";
            itemCursus -> appendRow(subItem);
        }
    }

}*/

const QList<Profil*> Cursus::getProfileList() const{
    if(PROlist.length() >0){
        return (const QList<Profil*>) PROlist;
    }else{
        QList<Profil*> ret;
        QList<const CreditType*> vide1;//= new QList<const CreditType*>;
        QList<const Portee*> vide2;// = new QList<const Portee*>;
        ValidationRule* foo = new XCreditsParmi(1,vide1,vide2,"(pas de règle de validation dans ce profil)");
        Profil* foo2 = new Profil("(pas de profil dans ce cursus)");
        foo2->addValidationRule(foo);
        ret<<foo2;
        return((const QList<Profil*>)ret);
    }
}

const QList<Cursus*> Cursus::getSOUSCursusList() const{
    if(SOUS_cursus.length() >0){
        return (const QList<Cursus*>) SOUS_cursus;
    }else{
        QList<Cursus*> ret ;
        //ret<<new Cursus("Pas de sous cursus");
        return ret;
    }
}

Cursus* Cursus::cloneRootElement() const{
    Cursus* ret = new Cursus(this->name);
    //ret.SOUS_cursus = QList<Cursus*>;
    ret->PROlist = this->PROlist;
    return ret;
}






void XUVParmi::testNullPtr(bool recursiv,bool debug=false)const{
    QString type=QString("XUVParmi");
    QString att1=QString("UVlist");
    if(debug)QMessageBox::warning(0," ",(string("\n")+this->getName()+string("\n\ntype=")+type.toStdString()+string(" att=")+att1.toStdString()+string(" len=")).c_str());  

    for(int i=0;i<UVlist.length();i++){     
        if(UVlist.at(i)==0)
            throw CursusNullPtrException(QString("!!Dans l'objet ")+QString(this->getName())+QString(" de type ")+type+QString(", il y a un pointeur nul dans l'attribut")+att1);
    }
    if(recursiv){};
}

void XCreditsParmi::testNullPtr(bool recursiv,bool debug=false)const{
    QString type=QString("XCreditsParmi");
    QString att1=QString("typelist");
    QString att2=QString("porteelist");
    if(debug)QMessageBox::warning(0," ",(string("\n")+this->getName()+string("\n\ntype=")+type.toStdString()+string(" att=")+att1.toStdString()+string(" len=")+string(" att2=")+att2.toStdString()+string(" len=")).c_str());    

    for(int i=0;i<typeList.length();i++){
        if(typeList.at(i)==0)
            throw CursusNullPtrException(QString("!!Dans l'objet ")+QString(this->getName())+QString(" de type ")+type+QString(", il y a un pointeur nul dans l'attribut")+att1);
    }
    for(int j=0;j<porteeList.length();j++){
        if(porteeList.at(j)==0)
            throw CursusNullPtrException(QString("!!Dans l'objet ")+QString(this->getName())+QString(" de type ")+type+QString(", il y a un pointeur nul dans l'attribut")+att2);
    }
    if(recursiv){};
}

void FonctionOU::testNullPtr(bool recursiv,bool debug=false)const{
    QString type=QString("FonctionOU");
    QString att1=QString("VRlist");
    
    if(debug)QMessageBox::warning(0," ",(string("\n")+this->getName()+string("\n\ntype=")+type.toStdString()+string(" att=")+att1.toStdString()+string(" len=")).c_str());    
    for(int i=0;i<VRlist.length();i++){
        if(VRlist.at(i)==0)
            throw CursusNullPtrException(QString("!!Dans l'objet ")+QString(this->getName())+QString(" de type ")+type+QString(", il y a un pointeur nul dans l'attribut")+att1);
        if(recursiv)
            VRlist.at(i)->testNullPtr(recursiv,debug);
    }
 }

 void FonctionET::testNullPtr(bool recursiv,bool debug=false)const{
    QString type=QString("FonctionET");
    QString att1=QString("VRlist");
    
    if(debug)QMessageBox::warning(0," ",(string("\n")+this->getName()+string("\n\ntype=")+type.toStdString()+string(" att=")+att1.toStdString()+string(" len=")).c_str());    
    for(int i=0;i<VRlist.length();i++){
        if(VRlist.at(i)==0)
            throw CursusNullPtrException(QString("!!Dans l'objet ")+QString(this->getName())+QString(" de type ")+type+QString(", il y a un pointeur nul dans l'attribut")+att1);
        if(recursiv)
            VRlist.at(i)->testNullPtr(recursiv,debug);
    }
 }

  void Profil::testNullPtr(bool recursiv,bool debug=false)const{
    QString type=QString("Profil");
    QString att1=QString("VRlist");
    
    if(debug)QMessageBox::warning(0," ",(string("\n")+this->getName()+string("\n\ntype=")+type.toStdString()+string(" att=")+att1.toStdString()+string(" len=")).c_str());    
    for(int i=0;i<VRlist.length();i++){
        if(VRlist.at(i)==0)
            throw CursusNullPtrException(QString("!!Dans l'objet ")+QString(this->getName())+QString(" de type ")+type+QString(", il y a un pointeur nul dans l'attribut")+att1);
        if(recursiv)
            VRlist.at(i)->testNullPtr(recursiv,debug);
    }
 }

 void Cursus::testNullPtr(bool recursiv,bool debug=false)const{
    QString type=QString("Cursus");
    QString att1=QString("PROlist");
    QString att2=QString("SOUS_cursus");
    if(debug)QMessageBox::warning(0," ",(string("\n")+this->getName()+string("\n\ntype=")+type.toStdString()+string(" att=")+att1.toStdString()+string(" len=")+string(" att2=")+att2.toStdString()+string(" len=")).c_str());    

    for(int i=0;i<PROlist.length();i++){
        if(PROlist.at(i)==0)
            throw CursusNullPtrException(QString("!!Dans l'objet ")+QString(this->getName())+QString(" de type ")+type+QString(", il y a un pointeur nul dans l'attribut")+att1);
        if(recursiv)
            PROlist.at(i)->testNullPtr(recursiv,debug);
    }
    for(int j=0;j<SOUS_cursus.length();j++){
        if(SOUS_cursus.at(j)==0)
            throw CursusNullPtrException(QString("!!Dans l'objet ")+QString(this->getName())+QString(" de type ")+type+QString(", il y a un pointeur nul dans l'attribut")+att2);
        if(recursiv)
            SOUS_cursus.at(j)->testNullPtr(recursiv,debug);
    }
}

 void Cursus_Etudiant::testNullPtr(bool recursiv,bool debug=false)const{
    QString type=QString("Cursus_Etudiant");
    QString att1=QString("PROlist");
    QString att2=QString("SOUS_cursus");
    if(debug)QMessageBox::warning(0," ",(string("\n")+this->getName()+string("\n\ntype=")+type.toStdString()+string(" att=")+att1.toStdString()+string(" len=")+string(" att2=")+att2.toStdString()+string(" len=")).c_str());    

    for(int i=0;i<PROlist.length();i++){
        if(PROlist.at(i)==0)
            throw CursusNullPtrException(QString("!!Dans l'objet ")+QString(this->getName())+QString(" de type ")+type+QString(", il y a un pointeur nul dans l'attribut")+att1);
        if(recursiv)
            PROlist.at(i)->testNullPtr(recursiv,debug);
    }
    for(int j=0;j<SOUS_cursus.length();j++){
        if(SOUS_cursus.at(j)==0)
            throw CursusNullPtrException(QString("!!Dans l'objet ")+QString(this->getName())+QString(" de type ")+type+QString(", il y a un pointeur nul dans l'attribut")+att2);
        if(recursiv)
            SOUS_cursus.at(j)->testNullPtr(recursiv,debug);
    }
}





/*
void XUVParmi::testNullPtr(bool recursiv)const{
}

void XCreditsParmi::testNullPtr(bool recursiv)const{
}

void FonctionOU::testNullPtr(bool recursiv)const{
 }

 void FonctionET::testNullPtr(bool recursiv)const{
 }

  void Profil::testNullPtr(bool recursiv)const{
 }

 void Cursus::testNullPtr(bool recursiv)const{
}

 void Cursus_Etudiant::testNullPtr(bool recursiv)const{
}
*/











/*void Dossier::ajouter_inscription(Saison sais,unsigned int annee, Note res, QString code){
    QString titre="titreàallerchercher";
    const UV u=new UV(code,titre);
    Inscription* nouvInsc= new Inscription(u,s,res);
    add_inscription(nouvInsc);
    modification=true;
}

void Dossier::add_inscription(Inscription* inscrip){

    inscr.push_back(inscrip);

}
void Credits::ajouterCredits(CreditType type ,unsigned int nb){


}
*/

//methodes à implémenter
/*;

void save();

void add_cursus();
void delete_cursus();
void add_equivalence();
void delete_equivalence();

void delete_inscription();
void add_semestre_etranger();
void delete_semestre_etranger();
void save_solution();
*/
