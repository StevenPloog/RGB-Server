from django.conf.urls import url
from . import views

app_name = 'rgb'
urlpatterns = [
    url(r'^$', views.index, name='index'),
    url(r'^web_input/', views.web_input, name='web_input'),
]

