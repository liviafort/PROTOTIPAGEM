import { Entity, PrimaryColumn, Column } from 'typeorm';

@Entity()
export class Wifi {
  @PrimaryColumn({ default: 1 })
  id: number;

  @Column()
  ssid: string;

  @Column()
  password: string;
}