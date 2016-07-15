from django.db import models
import uuid

class Routine(models.Model):
    readonly_fields = ('uuid',)
    
    name = models.CharField(max_length=32)
    category = models.CharField(max_length=32)
    uuid = models.UUIDField(primary_key=True, default=uuid.uuid4, editable=False)
    
    def __str__(self):
        return self.name

class Command(models.Model):
    CMD_CHOICES = (
        ('0', 'power'),
        ('1', 'rgb'),
        ('2', 'fade_time'),
        ('3', 'fade_color1'),
        ('4', 'fade_color2'),
    )
    routine = models.ForeignKey(Routine, on_delete=models.CASCADE)
    cmd = models.CharField(max_length=32, choices=CMD_CHOICES)
    red = models.IntegerField(default = 0)
    green = models.IntegerField(default = 0)
    blue = models.IntegerField(default = 0)
    def __str__(self):
        return self.cmd

