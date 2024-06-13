import pygame
pygame.init()

#screen = pygame.display.set_mode((2736, 1824))
screen = pygame.display.set_mode((1280, 720))
clock = pygame.time.Clock()

with open('output.csv', 'r') as doc:
  output_lines = doc.readlines()
with open('centroids.csv', 'r') as doc:
  centroid_lines = doc.readlines()

values = []
centroids = []
for v in output_lines:
  new_v = v.split(',')
  values.append((float(new_v[0]), float(new_v[1].replace('\n', ''))))

for c in centroid_lines:
  new_c = c.split(',')
  centroids.append((float(new_c[0]), float(new_c[1].replace('\n', ''))))


running = True
while running:

  for event in pygame.event.get():
    if event.type == pygame.QUIT:
      running = False
      
  for p in values:
    pygame.draw.circle(screen, (255, 0 , 0), p, 5)
  for p in centroids:
    pygame.draw.circle(screen, (0, 255 , 0), p, 10)
  
  
  pygame.display.flip()


  clock.tick(20)




pygame.quit()