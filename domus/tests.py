from django.test import TestCase
from django.core.urlresolvers import reverse

# homepage: it must return a status code 200
class IndexPageTestCase(TestCase):
    def test_index_page(self):
        response = self.client.get(reverse('index'))
        self.assertEqual(response.status_code, 200)
