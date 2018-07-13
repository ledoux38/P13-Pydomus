"""
Script that groups all the forms of the application.
"""
from django import forms

class Login(forms.Form):
    """
    Class for user account creation
    the user will ask:
    - Name
    - Password
    """
    password = forms.CharField(
        label='Mot de passe',
        required=True,
        widget=forms.PasswordInput(attrs={'class': 'form-control'}))

    name = forms.CharField(
        label='Nom:',
        max_length=32,
        required=True,
        widget=forms.TextInput(attrs={'class': 'form-control'}))



class Contact(forms.Form):
    """
    simple contact class
    """
    name = forms.CharField(
        label='Nom:',
        max_length=32,
        required=True,
        widget=forms.TextInput(attrs={'class': 'form-control'}))

    objet = forms.CharField(
        label='Objet:',
        max_length=32,
        required=True,
        widget=forms.TextInput(attrs={'class': 'form-control'}))

    email = forms.EmailField(
        label='Adresse e-mail:',
        required=True,
        widget=forms.EmailInput(attrs={'class': 'form-control'}))

    textArea = forms.CharField(
        label='Message',
        required=True,
        widget=forms.Textarea(attrs={'class': 'form-control'}))
