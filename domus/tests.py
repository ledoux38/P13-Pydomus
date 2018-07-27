from django.test import TestCase
from django.core.urlresolvers import reverse
from django.contrib.auth.models import User
from .models import *
import requests
from .utils import Crypthographie, convert_dict_to_list, convert_list_to_dict
import string

# homepage: it must return a status code 200
class IndexTestCase(TestCase):
    """test page index"""
    def setUp(self):
        self.user = User.objects.create_user('test', 'test@test.com', 'testpassword')
        self.param = Parameters.objects.create(url="http://pydomus.hopto.org:3000", key_parameters="NBVCXWMLKJHGFDSQPOIUYTREZA", key_value="AZERTYUIOP")

    def tearDown(self):
        pass

    def test_index_page(self):
        """the index.html page should be view only when the user and authenticated."""
        response = self.client.get(reverse('index'))
        self.assertEqual(response.status_code, 302)

    def test_login_page(self):
        """login.html page viewable without user authentication."""
        response = self.client.get(reverse('loginUser'))
        self.assertEqual(response.status_code, 200)

    def test_login_user(self):
        """test the display of the index window after authentication"""
        self.client.post(
            reverse('domus:loginUser'),
            {'name': 'test', 'password': 'testpassword'})
        response = self.client.get(reverse('index'))
        self.assertEqual(response.status_code, 200)


    def test_logout_user(self):
        self.client.post(
            reverse('domus:loginUser'),
            {'name': 'test', 'password': 'testpassword'})
        self.client.post(reverse('domus:logout'))
        response = self.client.get(reverse('index'))
        self.assertEqual(response.status_code, 302)


class mentionsLegalesPageTestCase(TestCase):
    """test page mentions legales """
    def test_mentions_legales_page(self):
        """referral code status test 200 from contact page"""
        response = self.client.get(reverse('mention_legales'))
        self.assertEqual(response.status_code, 200)

class convertDictToList(TestCase):
    """test def"""
    def setUp(self):
        """def"""
        self.dic = {'element': '9', 'type': '1', 'key': '1234', 'valeur': '1'}
        self.list = [['element', '9'], ['type', '1'], ['key', '1234'], ['valeur', '1']]


    def tearDown(self):
        """def"""
        pass

    def test_convertDictToList(self):
        """test function convertDictToList"""
        param = convert_dict_to_list(self.dic)
        self.assertEqual(len(self.list), len(param))

    def test_convertListToDict(self):
        """test function convertListToDict"""
        param = convert_list_to_dict(self.list)
        self.assertEqual(self.dic, param)

class TestCryptographie(TestCase):
    """test cryptage ansd decryptage"""
    def setUp(self):
        """def"""
        self.cryptage = Crypthographie()
        self.cryptage.add_key("param", string.ascii_lowercase, "NBVCXWMLKJHGFDSQPOIUYTREZA")
        self.cryptage.add_key("valeur", string.digits, "AZERTYUIOP")

    def tearDown(self):
        """def"""
        pass

    def test_cryptage(self):
        """test cryptage """
        param = [['valeur', '0'], ['type', '1'], ['element', '9'], ['key', '1234']]
        result = [['TNGXYO', 'A'], ['UZQX', 'Z'], ['XGXFXDU', 'P'], ['HXZ', 'ZERT']]

        container_crypt = []
        for i in param:
            p = self.cryptage.cryptage(i[0], "param")
            v = self.cryptage.cryptage(i[1], "valeur")
            container_crypt.append([p, v])
        self.assertEqual(container_crypt, result)

    def test_decryptage(self):
        """decryptage """
        result = [['valeur', '0'], ['type', '1'], ['element', '9'], ['key', '1234']]
        param = [['TNGXYO', 'A'], ['UZQX', 'Z'], ['XGXFXDU', 'P'], ['HXZ', 'ZERT']]

        container_crypt = []
        for i in param:
            p = self.cryptage.decryptage(i[0], "param")
            v = self.cryptage.decryptage(i[1], "valeur")
            container_crypt.append([p, v])
        self.assertEqual(container_crypt, result)
