from django.conf.urls import url
from . import views

app_name = 'rgb'
urlpatterns = [
    url(r'^$', views.index, name='index'),
#    url(r'^web_input_rgb/', views.web_input_rgb, name='web_input_rgb'),
    url(r'^web_input/', views.web_input, name='web_input'),
    url(r'^extras/', views.extras, name='extras'),
]

