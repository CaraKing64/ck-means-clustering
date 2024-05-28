import pygame


pygame.init()

#screen = pygame.display.set_mode((2736, 1824))
screen = pygame.display.set_mode((1280, 720))

clock = pygame.time.Clock()

ls = []
running = True
while running:

  for event in pygame.event.get():
    if event.type == pygame.QUIT:
      running = False
    if event.type == pygame.MOUSEBUTTONDOWN:
      ls.append(event.pos)
      print(event.pos)
      
  for p in ls:
    pygame.draw.circle(screen, (255, 0 , 0), p, 5)
  
  pygame.display.flip()


  clock.tick(20)



strls = []
for i in ls:
  strls.append(f"{i[0]},{i[1]}\n")

with open("output.csv", "w+") as doc:
  doc.writelines(strls)



pygame.quit()