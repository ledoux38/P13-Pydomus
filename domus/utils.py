"""
groups the different objects of the application
"""

class Crypthographie:
    """
    class that handles encryption and decryption
    """
    def __init__(self):
        """
        initialization class
        """
        self.m_list = dict()

    def cryptage(self, chaine, key):
        """
            value encryption method
        """
        if len(self.m_list) > 0:
            chaine_de_retour = ""
            for i in chaine:
                try:
                    index = self.m_list[key][0].index(i)
                except ValueError:
                    pass
                else:
                    chaine_de_retour += self.m_list[key][1][index]

            return chaine_de_retour
        else:
            raise ValueError("no key listed")


    def decryptage(self, chaine, key):
        """
            method of decrypting values
        """
        if len(self.m_list) > 0:
            chaine_de_retour = ""
            for i in chaine:
                try:
                    index = self.m_list[key][1].index(i)
                except ValueError:
                    pass
                else:
                    chaine_de_retour += self.m_list[key][0][index]

            return chaine_de_retour
        else:
            raise ValueError("no key listed")

    def add_key(self, identifiant, chaine_normale, chaine_converti):
        """
            Add a key to the object
        """
        if not isinstance(identifiant, str):
            raise TypeError("type identifiant != str")

        if not isinstance(chaine_normale, str):
            raise TypeError("type chaine_normale != str")

        if not isinstance(chaine_converti, str):
            raise TypeError("type chaine_converti != str")

        if not len(chaine_normale) == len(chaine_converti):
            raise ValueError("length chaine_normale != chaine_converti")
        self.m_list[identifiant] = [chaine_normale, chaine_converti]
