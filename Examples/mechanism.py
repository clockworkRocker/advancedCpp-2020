import tkinter as tk
import math

root_width, root_height = 700, 800  # size of the window


def transform_coordinates(coords):
    """
    Function transform coordinates in local c/s to the main c/s.

    :param coords: coordinates in local c/s
    :return: coordinates in the main c/s
    """

    xmain = int(root_width * 0.65)  # offset of the local c/s for x
    ymain = int(root_height)  # offset of the local c/s for y

    return [resizing(coords[0] + xmain), resizing(ymain - coords[1])]


def resizing(number):
    """
    Function used to resize elements in window if they are too big

    :param number: parameter needs to be resized
    :return: parameter, resized by special coefficient
    """

    coefficient = 0.7
    return number * coefficient


class Mechanism:

    def __init__(self, canvas, AB):
        self.filename = 'trajectory.txt'  # filename keeping list of angles
        self.canvas = canvas
        self.AB = AB  # length of AB (main parameter)
        self.BC = self.CE = self.DC = 2.17 * self.AB  # length of BC, CE, DC
        self.AD = 2.83 * self.AB  # length of AD
        self.BE = self.BC + self.CE  # length of BE, big stick)

        self.width = 5  # width of the sticks

        self.half_base_x = resizing(self.AB / 3.5)  # parameters for fixation, x
        self.half_base_y = resizing(self.AB / 2)  # parameters for fixation, y

        self.fi0 = 45  # angle between A and D
        self.fi1 = 0  # main angle fi1 in the scheme

        self.fi1_mass = [self.fi1]  # list to save all fi1 changes

        self.speed = 1  # increment of fi1

        self._calculate_fi()

        self.coordinates = {'A': [0, 0], 'B': [], 'C': [], 'D': [], 'E': []}  # dict, which have coordinates of all dots
        self.lines = dict()  # indexes of lines in canvas
        self.trajectory = dict()  # trajectories all dots

        self._calculate_coords()

        self.coordinates['D'].append(
            self.coordinates['C'][0] + self.DC * math.cos(self.fi3))  # x coordinate of D
        self.coordinates['D'].append(
            self.coordinates['C'][1] - self.DC * math.sin(self.fi3))  # y coordinate of D

        self.trajectory['E'] = [0, *self.coordinates['E']]
        self.trajectory['C'] = [0, *self.coordinates['C']]
        self.trajectory['B'] = [0, *self.coordinates['B']]

        self._draw_mechanism()
        self._write_file('w')

    def __del__(self):
        self._write_file('a')

    def _write_file(self, mode):
        """
        Method opens the file and inserts there lists of angles

        :param mode: Mode, which is used to open the file
        :return: None
        """

        with open(self.filename, mode, encoding='utf-8') as file:
            if mode == 'w':
                file.write('Стартовое значение угла fi1 = ' + str(self.fi1) + '\n')
                file.write('fi0 = ' + str(self.fi0) + '\n')
                file.write('fi1 увеличивается на ' + str(self.speed) + '\n')
            elif mode == 'a':
                file.write('fi1:\n')
                file.write(str(self.fi1_mass) + '\n')

                for dot in self.trajectory.keys():
                    file.write('Trajectory of {}, x:\n'.format(dot))
                    file.write(str(self.trajectory[dot][1::2]) + '\n')
                    file.write('Trajectory of {}, y:\n'.format(dot))
                    file.write(str(self.trajectory[dot][2::2]) + '\n')

                file.write('D = ' + str(self.coordinates['D']) + '\n')
                file.write('A = ' + str(self.coordinates['A']) + '\n')

    def _calculate_coords(self):
        """
        Method calculates coordinates of dots and saves them into self.coordinates. The center of c/s is dot A.

        :return: None
        """

        self.coordinates['B'].clear()
        self.coordinates['B'].append(
            self.AB * math.cos(math.radians(self.fi1)))  # x coordinate of B
        self.coordinates['B'].append(
            self.AB * math.sin(math.radians(self.fi1)))  # y coordinate of B

        self.coordinates['C'].clear()
        self.coordinates['C'].append(
            self.coordinates['B'][0] + self.BC * math.cos(self.fi2))  # x coordinate of C
        self.coordinates['C'].append(
            self.coordinates['B'][1] + self.BC * math.sin(self.fi2))  # y coordinate of C

        self.coordinates['E'].clear()
        self.coordinates['E'].append(
            self.coordinates['B'][0] + self.BE * math.cos(self.fi2))  # x coordinate of E
        self.coordinates['E'].append(
            self.coordinates['B'][1] + self.BE * math.sin(self.fi2))  # y coordinate of E

    def _draw_base(self, dot):
        """
        Method draws base on the certain dot

        :param dot: Dot's name
        :return: None
        """

        triangle = [(self.coordinates[dot][0], self.coordinates[dot][0]),
                    (self.coordinates[dot][0] - self.half_base_x,
                     self.coordinates[dot][0] - self.half_base_y),
                    (self.coordinates[dot][0] + self.half_base_x,
                     self.coordinates[dot][0] - self.half_base_y)]

        floor = [(self.coordinates[dot][0] - self.half_base_x * 2, self.coordinates[dot][0] - self.half_base_y),
                 (self.coordinates[dot][0] + self.half_base_x * 2, self.coordinates[dot][0] - self.half_base_y)]

        joint = [(self.coordinates[dot][0] - self.half_base_x / 3, self.coordinates[dot][0] + self.half_base_x / 3),
                 (self.coordinates[dot][0] + self.half_base_x / 3, self.coordinates[dot][0] - self.half_base_x / 3)]

        spike = [(self.coordinates[dot][0] - self.half_base_x * 1.6, self.coordinates[dot][0] - self.half_base_y),
                 (self.coordinates[dot][0] - self.half_base_x * 2, self.coordinates[dot][0] - self.half_base_y * 1.4)]

        triangle = list(map(lambda x: transform_coordinates(x), triangle))
        floor = list(map(lambda x: transform_coordinates(x), floor))
        joint = list(map(lambda x: transform_coordinates(x), joint))
        spike = list(map(lambda x: transform_coordinates(x), spike))

        self.canvas.create_polygon(*triangle, width=2, fill="white", outline="black")
        self.canvas.create_line(*floor, width=2)
        self.canvas.create_oval(*joint, width=2, fill="white")

        space = 15

        while spike[0][0] <= floor[1][0]:
            self.canvas.create_line(*spike, width=2)
            spike[0][0] += space
            spike[1][0] += space

    def _draw_mechanism(self):
        """
        Method creates lines of mechanism and saves indexes into self.lines.

        :return: None
        """

        self.lines['AB'] = self.canvas.create_line(transform_coordinates(self.coordinates['A']),
                                                   transform_coordinates(self.coordinates['B']),
                                                   width=self.width)
        self.lines['BC'] = self.canvas.create_line(transform_coordinates(self.coordinates['B']),
                                                   transform_coordinates(self.coordinates['C']),
                                                   width=self.width)
        self.lines['CE'] = self.canvas.create_line(transform_coordinates(self.coordinates['C']),
                                                   transform_coordinates(self.coordinates['E']),
                                                   width=self.width)
        self.lines['DC'] = self.canvas.create_line(transform_coordinates(self.coordinates['D']),
                                                   transform_coordinates(self.coordinates['C']),
                                                   width=self.width)

        A_coords = transform_coordinates(self.coordinates['A'])
        self.canvas.create_oval(A_coords[0] - resizing(self.AB),
                                A_coords[1] - resizing(self.AB),
                                A_coords[0] + resizing(self.AB),
                                A_coords[1] + resizing(self.AB), dash=(3, 5), width=1)

        self._draw_base('A')
        self._draw_base('D')

    def _calculate_fi(self):
        """
        Method calculates fi2 and fi3, using fi1 and fi0 values

        :return: None
        """

        part = 9.0089 - 5.66 * math.cos(math.radians(self.fi1 - self.fi0))
        arccos1 = math.acos(part / (4.34 * math.sqrt(part)))
        arcsin = math.asin(math.sin(math.radians(self.fi1 - self.fi0)) / math.sqrt(part))
        arccos2 = math.acos((0.4089 + 5.66 * math.cos(math.radians(self.fi1 - self.fi0))) / 9.4178)

        self.fi2 = (math.pi - arccos1 + math.radians(self.fi0) - arcsin - arccos2)
        self.fi3 = math.pi - (math.pi - arccos1 + math.radians(self.fi0) - arcsin)

    def _create_trajectory(self, dot, color):
        """
        Method creates the trajectory of certain dot, using special color

        :param dot: Dot's name
        :param color: color of the trajectory's line
        :return: None
        """

        line = self.trajectory[dot][1:]  # list for transformed coordinates

        for i in range(0, len(line) - 1, 2):  # transforming coordinates
            line[i], line[i + 1] = transform_coordinates((line[i], line[i + 1]))

        if not self.trajectory[dot][0]:
            self.trajectory[dot][0] = self.canvas.create_line(*line,
                                                              width=1, fill=color)
        else:
            self.canvas.coords(self.trajectory[dot][0], line)

    def move(self, event):
        """
        Method, invoked when angle fi1 changes, to change all the parameters of the mechanism

        :param event: Object with information about keyboard pressing
        :return: None
        """

        if event.keysym == 'Left':
            self.fi1 = (self.fi1 + self.speed) % 360
        else:
            self.fi1 = (self.fi1 - self.speed) % 360

        self.fi1_mass.append(round(self.fi1, 1))

        self._calculate_fi()

        self._calculate_coords()

        self.trajectory['E'].extend(self.coordinates['E'])
        self.trajectory['C'].extend(self.coordinates['C'])
        self.trajectory['B'].extend(self.coordinates['B'])

        self._create_trajectory('E', 'green')
        self._create_trajectory('C', 'red')

        for line in self.lines.keys():
            self.canvas.coords(self.lines[line],
                               *transform_coordinates(self.coordinates[line[0]]),
                               *transform_coordinates(self.coordinates[line[1]]))

    def clear_trajectory(self, dot):
        """
        Method deletes line of traectory from the canvas

        :param dot: Dot's name, whose traectory need to delete
        :return: None
        """

        self.canvas.delete(self.trajectory[dot][0])
        self.trajectory[dot].clear()
        self.trajectory[dot] = [0, *self.coordinates[dot]]


def main():
    root = tk.Tk()
    root.title("Визуализация механизма")

    canvas = tk.Canvas(root, width=root_width, height=(root_height - 100), bg='white')

    mechanism = Mechanism(canvas, 138.25)

    frm_buttons = tk.Frame(root, width=(root_width - 100), height=100, bg='white')
    frm_buttons.grid(row=1, column=0)

    btn_clear_C = tk.Button(frm_buttons,
                            text='Очистить траекторию\nточки С',
                            command=lambda: mechanism.clear_trajectory('C'))
    btn_clear_E = tk.Button(frm_buttons,
                            text='Очистить траекторию\nточки E',
                            command=lambda: mechanism.clear_trajectory('E'))

    canvas.grid(column=0, row=0, sticky=(tk.N, tk.W))
    btn_clear_C.grid(row=0, column=0, sticky="w")
    btn_clear_E.grid(row=0, column=1, sticky="e")

    root.bind('<KeyPress-Right>', mechanism.move)
    root.bind('<KeyPress-Left>', mechanism.move)

    root.mainloop()


if __name__ == '__main__':
    main()
