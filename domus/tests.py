from django.test import TestCase
from django.core.urlresolvers import reverse

# homepage: it must return a status code 200
class IndexTestCase(TestCase):
    """test page index"""
    def test_index_page(self):
        """referral code status test 200 from index page"""
        response = self.client.get(reverse('index'))
        self.assertEqual(response.status_code, 200)

class LoginPageTestCase(TestCase):
    """test page login """
    def test_login_page(self):
        """referral code status test 200 from login page"""
        response = self.client.get(reverse('login'))
        self.assertEqual(response.status_code, 200)

    def test_login_user(self):
        """descriptif text"""
        pass

class SettingsPageTestCase(TestCase):
    """test page setting """
    def test_settings_page(self):
        """referral code status test 200 from setting page"""
        response = self.client.get(reverse('settings'))
        self.assertEqual(response.status_code, 200)
