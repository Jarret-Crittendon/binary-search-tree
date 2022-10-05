#include <iostream>
#include "BST.hpp"

int main()
{
    BST<int> intbst;
    intbst.add(20);
    intbst.add(40);
    intbst.add(60);
    intbst.add(15);
    intbst.add(27);
    intbst.add(32);
    intbst.erase(15);
    intbst.add(17);
    intbst.add(10);
    intbst.add(13);
    intbst.add(19);

    intbst.inorder();

    std::cout << "\n\n";

    BST<std::string> idols;
    idols.add("KATAGIRI Sanae");
    idols.add("TACHIBANA Arisu");
    idols.add("SATOU Shin");
    idols.add("SASAKI Chie");
    idols.add("SAKURAI Momoka");
    idols.add("SHIJOU Takane");
    idols.add("MORIKUBO Nono");
    idols.add("TSUKIOKA Kogane");
    idols.add("MORINO Rinze");
    idols.add("AKIZUKI Ritsuko");
    idols.add("GANAHA Hibiki");
    idols.add("HOSHII Miki");
    idols.add("KANZAKI Ranko");
    idols.add("BABA Konomi");

    idols.inorder();
}