from django.db import models

# Create your models here.
class Parameters(models.Model):
    """
    representation of parameters in the server
    """
    url = models.TextField()
    port = models.TextField()
    key_parameters = models.TextField()
    key_value = models.TextField()
    key_identity = models.TextField()
