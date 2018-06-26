from django.test import TestCase
from django.core.urlresolvers import reverse
from django.contrib.auth.models import User
import requests
# homepage: it must return a status code 200
class IndexTestCase(TestCase):
    """test page index"""

    def setUp(self):
        user = User.objects.create_user('test', 'test@test.com', 'testpassword')

    def tearDown(self):
        pass

    def test_index_page(self):
        """the index.html page should be view only when the user and authenticated."""
        response = self.client.get(reverse('index'))
        self.assertEqual(response.status_code, 302)

    def test_login_page(self):
        """login.html page viewable without user authentication."""
        response = self.client.get(reverse('login'))
        self.assertEqual(response.status_code, 200)

    def test_login_user(self):
        """test the display of the index window after authentication"""
        # self.client.login(username='john', password='johnpassword')
        # response = self.client.get(reverse('index'))
        # self.assertEqual(response.status_code, 200)
        response = self.client.post(
            reverse('domus:login'),
            {'name': 'test', 'password': 'testpassword'})

class SettingsPageTestCase(TestCase):
    """test page setting """
    def test_settings_page(self):
        """referral code status test 200 from setting page"""
        response = self.client.get(reverse('settings'))
        self.assertEqual(response.status_code, 200)
