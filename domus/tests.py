from django.test import TestCase
from django.core.urlresolvers import reverse
from django.contrib.auth.models import User
import requests
# homepage: it must return a status code 200
class IndexTestCase(TestCase):
    """test page index"""

    def setUp(self):
        self.user = User.objects.create_user('test', 'test@test.com', 'testpassword')

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
        # self.client.login(username='john', password='johnpassword')
        # response = self.client.get(reverse('index'))
        # self.assertEqual(response.status_code, 200)
        self.client.post(
            reverse('domus:loginUser'),
            {'name': 'test', 'password': 'testpassword'})
        response = self.client.get(reverse('index'))
        self.assertEqual(response.status_code, 200)

    def test_settings_page(self):
        """referral code status test 200 from setting page"""
        self.client.post(
            reverse('domus:loginUser'),
            {'name': 'test', 'password': 'testpassword'})
        response = self.client.get(reverse('settings'))
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

class ContactPageTestCase(TestCase):
    """test page contact """
    def test_contact_page(self):
        """referral code status test 200 from contact page"""
        response = self.client.get(reverse('contact'))
        self.assertEqual(response.status_code, 200)
